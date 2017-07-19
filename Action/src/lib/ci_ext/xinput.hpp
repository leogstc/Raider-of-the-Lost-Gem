//--------------------------------------------------------------------------------------------
//  作成者 : 植山沙欧
//  機能   : XInputを隠蔽する汎用的関数群です。
//           XBOX360に準拠したパッドを使う際に使用できます。
//  使用方法  : 毎フレームgplib::xinput::CheckKey()を呼んで、内部の入力データ更新を行ってください。
//              394-489行目のラッパー関数を使うことでgplib::inputの関数とほぼ同じ動作をするように組んであります。
//              振動やアナログキーなどの実装もされています。
//  実装クラス: Controller  パッド単体のコンポーネント
//              XInput    4つのパッドの管理、ボタンID設定、
//              シングルトンモデルで作成しているので、このクラスのみで運用可能
//              
//              オブジェクトのポインタはXInput::GetInstance()で取得可能
//              <例>
//              auto p = XInput::GetInstance();
//              if(p->checkPush(XInput::ButtonsID::A))
//              
//              staticメソッドを用意しているので、直接呼出しも可能
//              <例>
//              if(XInput::CheckPush(XInput::ButtonsID::A))
//
//  更新履歴  ：2014.09.30  Ver1.00  植山沙欧  基本的な機能の実装
//              2014.11.24  Ver1.01  植山沙欧  namespaceのxinputを廃止し、XInputクラスのstaticメソッドとした
//              2015.05.25  Ver1.02  植山沙欧  フレーム数を指定してvivrationできるようにした
//--------------------------------------------------------------------------------------------

#pragma once

#pragma comment(lib, "xinput.lib")
#include <XInput.h> 
#include <d3dx9.h> 
#include <vector>
#include <array>

namespace gplib
{

//Xinput関連
class XInput
{
public:
  //XInput互換パッドのボタン定義
  //コメントにデジタルとあるのはOnOffのみ判定でき、
  //アナログとあるのは0.0f~1.0fの間で取得できる
  enum class ButtonsID
  {
    //十字キー
    up = 0,     //デジタル上
    down = 1,   //デジタル下
    left = 2,   //デジタル左
    right = 3,  //デジタル右
    //スタート、バック
    start = 4,  //デジタルスタート
    back = 5,   //デジタルバック
    //アナログLR
    L_thumb = 6,//アナログL
    R_thumb = 7,//アナログR
    //トリガー
    L = 9,      //デジタルL
    R = 10,     //デジタルR
    A = 12,     //デジタルA
    B = 13,     //デジタルB
    X = 14,     //デジタルX
    Y = 15,     //デジタルY
    max
  };
  //XInputでは、最大４つまでのコントローラしか認識できないので、IDを増やしてもダメ
  enum class ID
  {
    controller_1P = 0,  //1PパッドのID
    controller_2P = 1,  //2PパッドのID
    controller_3P = 2,  //3PパッドのID
    controller_4P = 3,  //4PパッドのID
    max_controllers = 4 //パッドは4つまで
  };


  //--------------------------------------------------------------------------------------------
  //コントローラ部共通処理
  //--------------------------------------------------------------------------------------------
  class ControllerBase
  {
  protected:

    int id_;  //コントローラのID（0から3まで）

    WORD now_,//現フレームでのボタン情報
      prev_;  //前フレームでのボタン情報

    //ボタン情報
    struct ButtonState
    {
      int holdframe;//おされているフレーム数
      int pullframe;//はなされているフレーム数
    } framestate_[static_cast<int>(ButtonsID::max)];

  public:
    //デフォルトコンストラクタ消去
    //コンパイルエラー出るときは、=deleteを{}に変更しprivate:へ
    ControllerBase() = delete;

    //コントローラ初期化処理
    //　id　初期化対象のコントローラIDを指定
    ControllerBase(ID id)
      :
      now_(0),
      prev_(0),
      id_(static_cast<int>(id))
    {
      for (auto& framestate : framestate_)
        framestate.holdframe = framestate.pullframe = 0;
    }

    //未使用
    virtual void render() { }

    //共通のupdate
    virtual void update() = 0;

    virtual void update_common()
    {
      //各ボタンに対し、おされているフレームはなされているフレームをカウント
      for (int i = 0; i < static_cast<int>(ButtonsID::max); ++i)
      {
        ButtonsID buttonID = static_cast<ButtonsID>(i);
        //このフレームで対象ボタンがおされたかはなされたかでフレームカウンタを0クリア
        if (checkPush(buttonID) || checkPull(buttonID))
        {
          framestate_[i].holdframe = -1;
          framestate_[i].pullframe = -1;
        }
        //おされているかはなされているかでフレームカウンタをインクリメント
        if (checkPress(buttonID)) ++framestate_[i].holdframe;
        if (checkFree(buttonID))  ++framestate_[i].pullframe;
      }
    }

    //押されているかのチェック
    virtual bool checkPress(const ButtonsID btn, int interval = 1) const
    {
      WORD flag = 1 << static_cast<int>(btn);
      return ((now_ & flag) == flag && (framestate_[static_cast<int>(btn)].holdframe % interval) == 0);
    }

    //押されていないかのチェック
    virtual bool checkFree(const ButtonsID btn, int interval = 1)  const
    {
      WORD flag = 1 << static_cast<int>(btn);
      return ((now_ & flag) == 0 && (framestate_[static_cast<int>(btn)].pullframe % interval) == 0);
    }

    //押されたかのチェック
    virtual bool checkPush(const ButtonsID btn)  const
    {
      WORD flag = 1 << static_cast<int>(btn);
      return ((prev_ & flag) == 0) && ((now_ & flag) != 0);
    }

    //はなされたかのチェック
    virtual bool checkPull(const ButtonsID btn)  const
    {
      WORD flag = 1 << static_cast<int>(btn);
      return ((prev_ & flag) != 0) && ((now_ & flag) == 0);
    }

    //トリガースティック左の情報を取得する
    //0.0~1.0で返ってくる
    virtual float leftTrigger() const = 0;

    //トリガースティック右の情報を取得する
    //0.0~1.0で返ってくる
    virtual float rightTrigger() const = 0;

    //アナログスティック左の情報を取得する
    //1.0~-1.0で返ってくる
    virtual D3DXVECTOR2 leftThumb() const = 0;

    //アナログスティック右の情報を取得する
    //1.0~-1.0で返ってくる
    virtual D3DXVECTOR2 rightThumb() const = 0;

    //パッドを振動させる
    //　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
    //　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
    virtual void vibration(float leftpower, float rightpower) {}

    //パッドを振動させる
    //　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
    virtual void vibrationLeft(float leftpower) {}

    //パッドを振動させる
    //　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
    virtual void vibrationRight(float rightpower) {}

    //パッドを振動させる
    //　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
    //　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
    virtual void vibrationFrame(float leftpower, float rightpower, int frame) {}

    //パッドを振動させる
    //　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
    virtual void vibrationLeftFrame(float leftpower, int frame) {}

    //パッドを振動させる
    //　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
    virtual void vibrationRightFrame(float rightpower, int frame) {}

  };


  //--------------------------------------------------------------------------------------------
  //XInput対応パッド処理
  //--------------------------------------------------------------------------------------------
  class Controller : public ControllerBase
  {
  private:
    const float deadzone_ = static_cast<float>(0x7FFF) * 0.24f;//ニュートラル時のあそび値の範囲  microsoftのデフォルト値が24%指定なのでこうなった

    XINPUT_STATE state_;//ボタン情報
    XINPUT_VIBRATION vibration_;//ぶるぶる情報
    enum Motor
    {
      left, right, motornum
    };
    std::array<int, motornum> vibrationFrame_;
    bool connected_;//コネクトされているか

    //アナログパッド情報の取得
    D3DXVECTOR2 getThumb(const short xx, const short yy) const
    {
      if (!connected_) return D3DXVECTOR2(0, 0);
      float x = xx >= 0 ? 32767.0f : 32768.0f;
      float y = yy >= 0 ? 32767.0f : 32768.0f;
      return D3DXVECTOR2(
        static_cast<float>(xx) / x,
        static_cast<float>(yy) / y
        );
    }


  public:
    //デフォルトコンストラクタ消去
    //コンパイルエラー出るときは、=deleteを{}に変更しprivate:へ
    Controller() = delete;

    //コントローラ初期化処理
    //　id　初期化対象のコントローラIDを指定
    Controller(ID id)
      :
      ControllerBase(id),
      state_(),
      vibration_(),
      connected_(false)
    {
      vibrationFrame_.fill(-1);
    }

    //パッド用update
    void update() override
    {
      //前回のチェック
      prev_ = now_;

      //パッド抜けさしチェック
      if (XInputGetState(id_, &state_) != ERROR_SUCCESS)
      {
        connected_ = false;
        now_ = 0;
        return;
      }

      connected_ = true;

      //現在のボタン情報の取得
      now_ = state_.Gamepad.wButtons;

      //ニュートラル時のあそび値を消去させる（左）
      if ((state_.Gamepad.sThumbLX < deadzone_ &&
        state_.Gamepad.sThumbLX > -deadzone_) &&
        (state_.Gamepad.sThumbLY < deadzone_ &&
        state_.Gamepad.sThumbLY > -deadzone_))
      {
        state_.Gamepad.sThumbLX = 0;
        state_.Gamepad.sThumbLY = 0;
      }

      //ニュートラル時のあそび値を消去させる（右）
      if ((state_.Gamepad.sThumbRX < deadzone_ &&
        state_.Gamepad.sThumbRX > -deadzone_) &&
        (state_.Gamepad.sThumbRY < deadzone_ &&
        state_.Gamepad.sThumbRY > -deadzone_))
      {
        state_.Gamepad.sThumbRX = 0;
        state_.Gamepad.sThumbRY = 0;
      }

      update_common();

      //モーターの停止を行う
      if (vibrationFrame_[Motor::left] >= 0)
      {
        if (--vibrationFrame_[Motor::left] < 0)
          vibrationLeft(0);
      }
      if (vibrationFrame_[Motor::right] >= 0)
      {
        if (--vibrationFrame_[Motor::right] < 0)
          vibrationRight(0);
      }
    }

    //押されているかのチェック
    bool checkPress(const ButtonsID btn, int interval = 1) const override
    {
      if (!connected_) return false;
      return ControllerBase::checkPress(btn, interval);
    }

    //押されていないかのチェック
    bool checkFree(const ButtonsID btn, int interval = 1)  const override
    {
      if (!connected_) return false;
      return ControllerBase::checkFree(btn, interval);
    }

    //押されたかのチェック
    bool checkPush(const ButtonsID btn)  const override
    {
      if (!connected_) return false;
      return ControllerBase::checkPush(btn);
    }

    //はなされたかのチェック
    bool checkPull(const ButtonsID btn)  const override
    {
      if (!connected_) return false;
      return ControllerBase::checkPull(btn);
    }

    //トリガースティック左の情報を取得する
    //0.0~1.0で返ってくる
    float leftTrigger() const override
    {
      if (!connected_) return false;
      return static_cast<float>(state_.Gamepad.bLeftTrigger) / 255.0f;
    }

    //トリガースティック右の情報を取得する
    //0.0~1.0で返ってくる
    float rightTrigger() const override
    {
      if (!connected_) return false;
      return static_cast<float>(state_.Gamepad.bRightTrigger) / 255.0f;
    }

    //アナログスティック左の情報を取得する
    //1.0~-1.0で返ってくる
    D3DXVECTOR2 leftThumb() const override
    {
      return getThumb(state_.Gamepad.sThumbLX, state_.Gamepad.sThumbLY);
    }

    //アナログスティック右の情報を取得する
    //1.0~-1.0で返ってくる
    D3DXVECTOR2 rightThumb() const override
    {
      return getThumb(state_.Gamepad.sThumbRX, state_.Gamepad.sThumbRY);
    }

    //パッドを振動させる
    //　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
    //　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
    void vibration(float leftpower, float rightpower) override
    {
      vibration_.wLeftMotorSpeed = static_cast<WORD>(65535.0f * leftpower);
      vibration_.wRightMotorSpeed = static_cast<WORD>(65535.0f * rightpower);
      XInputSetState(id_, &vibration_);
    }
    //パッドを振動させる
    //　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
    void vibrationLeft(float leftpower) override
    {
      vibration_.wLeftMotorSpeed = static_cast<WORD>(65535.0f * leftpower);
      XInputSetState(id_, &vibration_);
    }
    //パッドを振動させる
    //　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
    void vibrationRight(float rightpower) override
    {
      vibration_.wRightMotorSpeed = static_cast<WORD>(65535.0f * rightpower);
      XInputSetState(id_, &vibration_);
    }

    //パッドを振動させる
    //　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
    //　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
    void vibrationFrame(float leftpower, float rightpower, int frame) override
    {
      vibration(leftpower, rightpower);
      vibrationFrame_[Motor::left] = frame;
      vibrationFrame_[Motor::right] = frame;
    }
    //パッドを振動させる
    //　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
    void vibrationLeftFrame(float leftpower, int frame) override
    {
      vibrationLeft(leftpower);
      vibrationFrame_[Motor::left] = frame;
    }
    //パッドを振動させる
    //　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
    void vibrationRightFrame(float rightpower, int frame) override
    {
      vibrationRight(rightpower);
      vibrationFrame_[Motor::right] = frame;
    }
  };

  //--------------------------------------------------------------------------------------------
  //キーボード処理
  //--------------------------------------------------------------------------------------------
  class KeyboardController : public ControllerBase
  {
  private:
    std::array<int, static_cast<int>(ButtonsID::max)> keymap_;
    void setDefaultKeyboardMap()
    {
      keymap_[static_cast<int>(ButtonsID::up)] = VK_UP;
      keymap_[static_cast<int>(ButtonsID::down)] = VK_DOWN;
      keymap_[static_cast<int>(ButtonsID::left)] = VK_LEFT;
      keymap_[static_cast<int>(ButtonsID::right)] = VK_RIGHT;
      keymap_[static_cast<int>(ButtonsID::start)] = VK_RETURN;
      keymap_[static_cast<int>(ButtonsID::back)] = VK_BACK;
      keymap_[static_cast<int>(ButtonsID::L_thumb)] = 'Z';
      keymap_[static_cast<int>(ButtonsID::R_thumb)] = 'C';
      keymap_[8] = 0;
      keymap_[static_cast<int>(ButtonsID::L)] = 'Q';
      keymap_[static_cast<int>(ButtonsID::R)] = 'E';
      keymap_[11] = 0;
      keymap_[static_cast<int>(ButtonsID::A)] = 'S';
      keymap_[static_cast<int>(ButtonsID::B)] = 'D';
      keymap_[static_cast<int>(ButtonsID::X)] = 'A';
      keymap_[static_cast<int>(ButtonsID::Y)] = 'W';
    }

  public:
    //デフォルトコンストラクタ消去
    //コンパイルエラー出るときは、=deleteを{}に変更しprivate:へ
    KeyboardController() = delete;

    //コントローラ初期化処理
    //　id　初期化対象のコントローラIDを指定
    KeyboardController(ID id)
      :
      ControllerBase(id)
    {
      setDefaultKeyboardMap();
    }

    void setKeyboardMap(const std::array<int, static_cast<int>(ButtonsID::max)>& newmap)
    {
      for (size_t i = 0; i < keymap_.size(); ++i)
        keymap_[i] = newmap[i];
    }

    std::array<int, static_cast<int>(ButtonsID::max)> getKeyboardMap() const
    {
      return keymap_;
    }
    
    //キーボード用update
    void update() override
    {
      //前回のチェック
      prev_ = now_;

      //各ボタンの状態を更新
      now_ = 0;
      for (int i = 0; i < static_cast<int>(ButtonsID::max); ++i)
      {
        WORD flag = (1 << i);
        if (GetAsyncKeyState(keymap_[i]))
          now_ |= flag;
      }

      update_common();
    }

    //トリガースティック左の情報を取得する
    //0.0~1.0で返ってくる
    float leftTrigger() const override
    {
      return 0;
    }

    //トリガースティック右の情報を取得する
    //0.0~1.0で返ってくる
    float rightTrigger() const override
    {
      return 0;
    }

    //アナログスティック左の情報を取得する
    //1.0~-1.0で返ってくる
    D3DXVECTOR2 leftThumb() const override
    {
      int x = 1, y = 1;
      float radian[3][3] = 
      { 
        { 3.0f, 2.0f, 1.0f },
        { 4.0f, 0.0f, 0.0f },
        { -3.0f, -2.0f, -1.0f }
      };
      if (checkPress(ButtonsID::left))
        x = 0;
      else if (checkPress(ButtonsID::right))
        x = 2;
      if (checkPress(ButtonsID::up))
        y = 0;
      else if (checkPress(ButtonsID::down))
        y = 2;

      if (x == 1 && y == 1)
        return D3DXVECTOR2(0,0);

      return D3DXVECTOR2(cos(radian[y][x] / 4.0f*D3DX_PI), sin(radian[y][x] / 4.0f*D3DX_PI));
    }

    //アナログスティック右の情報を取得する
    //1.0~-1.0で返ってくる
    D3DXVECTOR2 rightThumb() const override
    {
      return D3DXVECTOR2(0, 0);
    }
  };

private:

  std::vector<std::unique_ptr<ControllerBase>> controllers_;

  //各種コンストラクタ封印
  //関数ヘルパーのためにシングルトンモデルにしたため
  XInput(const XInput&){}
  XInput(const XInput&&){}
  XInput()
  {
    controllers_.clear();
    controllers_.push_back(std::make_unique<Controller>(ID::controller_1P));
    controllers_.push_back(std::make_unique<Controller>(ID::controller_2P));
    controllers_.push_back(std::make_unique<Controller>(ID::controller_3P));
    controllers_.push_back(std::make_unique<Controller>(ID::controller_4P));
  }

public:
  //インスタンス取得
  static XInput* GetInstance()
  {
    static XInput instance;
    return &instance;
  }

  //毎フレーム呼ぶ
  void update()
  {
    for (auto& controller : controllers_)
      controller->update();
  }

  //描画（なにもしない）
  void render()
  {
    for (auto& controller : controllers_)
      controller->render();
  }

  //押されているかのチェック
  bool checkPress(const ButtonsID btn, int interval = 1, ID id = ID::controller_1P) const
  {
    return controllers_[static_cast<int>(id)]->checkPress(btn, interval);
  }

  //押されていないかのチェック
  bool checkFree(const ButtonsID btn, int interval = 1, ID id = ID::controller_1P)  const
  {
    return controllers_[static_cast<int>(id)]->checkFree(btn, interval);
  }

  //押されたかのチェック
  bool checkPush(const ButtonsID btn, ID id = ID::controller_1P)  const
  {
    return controllers_[static_cast<int>(id)]->checkPush(btn);
  }

  //はなされたかのチェック
  bool checkPull(const ButtonsID btn, ID id = ID::controller_1P)  const
  {
    return controllers_[static_cast<int>(id)]->checkPull(btn);
  }

  //1P~4Pまでのどれかのボタンが押されているかをチェック
  bool checkAnyPress(const ButtonsID btn) const
  {
    for (const auto& controller : controllers_)
      if (controller->checkPress(btn, 1)) return true;
    return false;
  }

  //1P~4Pまでのどれかのボタンが押されたかをチェック
  bool checkAnyPush(const ButtonsID btn) const
  {
    for (const auto& controller : controllers_)
      if (controller->checkPush(btn)) return true;
    return false;
  }

  //1P~4Pまでのどれかのボタンがはなされているかをチェック
  bool checkAnyFree(const ButtonsID btn) const
  {
    for (const auto& controller : controllers_)
      if (controller->checkFree(btn, 1)) return true;
    return false;
  }

  //1P~4Pまでのどれかのボタンがはなされたかをチェック
  bool checkAnyPull(const ButtonsID btn) const
  {
    for (const auto& controller : controllers_)
      if (controller->checkPull(btn)) return true;
    return false;
  }

  //トリガースティック左の情報を取得する
  //0.0~1.0で返ってくる
  float leftTrigger(ID id = ID::controller_1P) const
  {
    return controllers_[static_cast<int>(id)]->leftTrigger();
  }

  //トリガースティック右の情報を取得する
  //0.0~1.0で返ってくる
  float rightTrigger(ID id = ID::controller_1P) const
  {
    return controllers_[static_cast<int>(id)]->rightTrigger();
  }

  //アナログスティック左の情報を取得する
  //1.0~-1.0で返ってくる
  D3DXVECTOR2 leftThumb(ID id = ID::controller_1P) const
  {
    return controllers_[static_cast<int>(id)]->leftThumb();
  }

  //アナログスティック右の情報を取得する
  //1.0~-1.0で返ってくる
  D3DXVECTOR2 rightThumb(ID id = ID::controller_1P) const
  {
    return controllers_[static_cast<int>(id)]->rightThumb();
  }

  //パッドを振動させる
  //　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
  //　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
  void vibration(float leftpower, float rightpower, ID id = ID::controller_1P)
  {
    controllers_[static_cast<int>(id)]->vibration(leftpower, rightpower);
  }

  //パッドを振動させる
  //　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
  void vibrationLeft(float leftpower, ID id = ID::controller_1P)
  {
    controllers_[static_cast<int>(id)]->vibrationLeft(leftpower);
  }

  //パッドを振動させる
  //　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
  void vibrationRight(float rightpower, ID id = ID::controller_1P)
  {
    controllers_[static_cast<int>(id)]->vibrationRight(rightpower);
  }

  //パッドを振動させる
  //　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
  //　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
  void vibrationFrame(float leftpower, float rightpower, int frame, ID id = ID::controller_1P)
  {
    controllers_[static_cast<int>(id)]->vibrationFrame(leftpower, rightpower, frame);
  }

  //パッドを振動させる
  //　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
  void vibrationLeftFrame(float leftpower, int frame, ID id = ID::controller_1P)
  {
    controllers_[static_cast<int>(id)]->vibrationLeftFrame(leftpower, frame);
  }

  //パッドを振動させる
  //　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
  void vibrationRightFrame(float rightpower, int frame, ID id = ID::controller_1P)
  {
    controllers_[static_cast<int>(id)]->vibrationRightFrame(rightpower, frame);
  }

  //----------------------------------------------------------------------------------------
  //関数のように使用するためのラッパ関数郡
  //毎フレーム呼びましょう
  static void CheckKey()
  {
    XInput::GetInstance()->update();
  }

  //押されているかチェック
  static bool CheckPress(const XInput::ButtonsID buttonID, int interval = 1, XInput::ID id = XInput::ID::controller_1P)
  {
    return XInput::GetInstance()->checkPress(buttonID, interval, id);
  }

  //はなされているかチェック
  static bool CheckFree(const XInput::ButtonsID buttonID, int interval = 1, XInput::ID id = XInput::ID::controller_1P)
  {
    return XInput::GetInstance()->checkFree(buttonID, interval, id);
  }

  //押されたかのチェック
  static bool CheckPush(const XInput::ButtonsID btn, XInput::ID id = XInput::ID::controller_1P)
  {
    return XInput::GetInstance()->checkPush(btn);
  }

  //はなされたかのチェック
  static bool CheckPull(const XInput::ButtonsID btn, XInput::ID id = XInput::ID::controller_1P)
  {
    return XInput::GetInstance()->checkPull(btn, id);
  }

  //1P~4Pまでのどれかのボタンが押されているかをチェック
  static bool CheckAnyPress(const XInput::ButtonsID btn)
  {
    return XInput::GetInstance()->checkAnyPress(btn);
  }

  //1P~4Pまでのどれかのボタンが押されているかをチェック
  static bool CheckAnyPush(const XInput::ButtonsID btn)
  {
    return XInput::GetInstance()->checkAnyPush(btn);
  }

  //1P~4Pまでのどれかのボタンがはなされているかをチェック
  static bool CheckAnyFree(const XInput::ButtonsID btn)
  {
    return XInput::GetInstance()->checkAnyFree(btn);
  }

  //1P~4Pまでのどれかのボタンがはなされたかをチェック
  static bool CheckAnyPull(const XInput::ButtonsID btn)
  {
    return XInput::GetInstance()->checkAnyPull(btn);
  }

  //トリガースティック左の情報を取得する
  //0.0~1.0で返ってくる
  static float LeftTrigger(XInput::ID id = XInput::ID::controller_1P)
  {
    return XInput::GetInstance()->leftTrigger(id);
  }

  //トリガースティック右の情報を取得する
  //0.0~1.0で返ってくる
  static float RightTrigger(XInput::ID id = XInput::ID::controller_1P)
  {
    return XInput::GetInstance()->rightTrigger(id);
  }

  //アナログスティック左の情報を取得する
  //1.0~-1.0で返ってくる
  static D3DXVECTOR2 LeftThumb(XInput::ID id = XInput::ID::controller_1P)
  {
    return XInput::GetInstance()->leftThumb(id);
  }

  //アナログスティック右の情報を取得する
  //1.0~-1.0で返ってくる
  static D3DXVECTOR2 RightThumb(XInput::ID id = XInput::ID::controller_1P)
  {
    return XInput::GetInstance()->rightThumb(id);
  }

  //パッドを振動させる
  //　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
  //　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
  static void Vibration(float leftpower, float rightpower, XInput::ID id = XInput::ID::controller_1P)
  {
    XInput::GetInstance()->vibration(leftpower, rightpower, id);
  }

  //パッドを振動させる
  //　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
  static void VibrationLeft(float leftpower, XInput::ID id = XInput::ID::controller_1P)
  {
    XInput::GetInstance()->vibrationLeft(leftpower, id);
  }

  //パッドを振動させる
  //　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
  static void VibrationRight(float rightpower, XInput::ID id = XInput::ID::controller_1P)
  {
    XInput::GetInstance()->vibrationRight(rightpower, id);
  }

  //パッドを指定フレーム分振動させる
  //　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
  //　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
  static void VibrationFrame(float leftpower, float rightpower, int frame, XInput::ID id = XInput::ID::controller_1P)
  {
    XInput::GetInstance()->vibrationFrame(leftpower, rightpower, frame, id);
  }

  //パッドを振動させる
  //　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
  static void VibrationLeftFrame(float leftpower, int frame, XInput::ID id = XInput::ID::controller_1P)
  {
    XInput::GetInstance()->vibrationLeftFrame(leftpower, frame, id);
  }

  //パッドを振動させる
  //　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
  static void VibrationRightFrame(float rightpower, int frame, XInput::ID id = XInput::ID::controller_1P)
  {
    XInput::GetInstance()->vibrationRightFrame(rightpower, frame, id);
  }
};


}