//--------------------------------------------------------------------------------------------
//  �쐬�� : �A�R����
//  �@�\   : XInput���B������ėp�I�֐��Q�ł��B
//           XBOX360�ɏ��������p�b�h���g���ۂɎg�p�ł��܂��B
//  �g�p���@  : ���t���[��gplib::xinput::CheckKey()���Ă�ŁA�����̓��̓f�[�^�X�V���s���Ă��������B
//              394-489�s�ڂ̃��b�p�[�֐����g�����Ƃ�gplib::input�̊֐��Ƃقړ������������悤�ɑg��ł���܂��B
//              �U����A�i���O�L�[�Ȃǂ̎���������Ă��܂��B
//  �����N���X: Controller  �p�b�h�P�̂̃R���|�[�l���g
//              XInput    4�̃p�b�h�̊Ǘ��A�{�^��ID�ݒ�A
//              �V���O���g�����f���ō쐬���Ă���̂ŁA���̃N���X�݂̂ŉ^�p�\
//              
//              �I�u�W�F�N�g�̃|�C���^��XInput::GetInstance()�Ŏ擾�\
//              <��>
//              auto p = XInput::GetInstance();
//              if(p->checkPush(XInput::ButtonsID::A))
//              
//              static���\�b�h��p�ӂ��Ă���̂ŁA���ڌďo�����\
//              <��>
//              if(XInput::CheckPush(XInput::ButtonsID::A))
//
//  �X�V����  �F2014.09.30  Ver1.00  �A�R����  ��{�I�ȋ@�\�̎���
//              2014.11.24  Ver1.01  �A�R����  namespace��xinput��p�~���AXInput�N���X��static���\�b�h�Ƃ���
//              2015.05.25  Ver1.02  �A�R����  �t���[�������w�肵��vivration�ł���悤�ɂ���
//--------------------------------------------------------------------------------------------

#pragma once

#pragma comment(lib, "xinput.lib")
#include <XInput.h> 
#include <d3dx9.h> 
#include <vector>
#include <array>

namespace gplib
{

//Xinput�֘A
class XInput
{
public:
  //XInput�݊��p�b�h�̃{�^����`
  //�R�����g�Ƀf�W�^���Ƃ���̂�OnOff�̂ݔ���ł��A
  //�A�i���O�Ƃ���̂�0.0f~1.0f�̊ԂŎ擾�ł���
  enum class ButtonsID
  {
    //�\���L�[
    up = 0,     //�f�W�^����
    down = 1,   //�f�W�^����
    left = 2,   //�f�W�^����
    right = 3,  //�f�W�^���E
    //�X�^�[�g�A�o�b�N
    start = 4,  //�f�W�^���X�^�[�g
    back = 5,   //�f�W�^���o�b�N
    //�A�i���OLR
    L_thumb = 6,//�A�i���OL
    R_thumb = 7,//�A�i���OR
    //�g���K�[
    L = 9,      //�f�W�^��L
    R = 10,     //�f�W�^��R
    A = 12,     //�f�W�^��A
    B = 13,     //�f�W�^��B
    X = 14,     //�f�W�^��X
    Y = 15,     //�f�W�^��Y
    max
  };
  //XInput�ł́A�ő�S�܂ł̃R���g���[�������F���ł��Ȃ��̂ŁAID�𑝂₵�Ă��_��
  enum class ID
  {
    controller_1P = 0,  //1P�p�b�h��ID
    controller_2P = 1,  //2P�p�b�h��ID
    controller_3P = 2,  //3P�p�b�h��ID
    controller_4P = 3,  //4P�p�b�h��ID
    max_controllers = 4 //�p�b�h��4�܂�
  };


  //--------------------------------------------------------------------------------------------
  //�R���g���[�������ʏ���
  //--------------------------------------------------------------------------------------------
  class ControllerBase
  {
  protected:

    int id_;  //�R���g���[����ID�i0����3�܂Łj

    WORD now_,//���t���[���ł̃{�^�����
      prev_;  //�O�t���[���ł̃{�^�����

    //�{�^�����
    struct ButtonState
    {
      int holdframe;//������Ă���t���[����
      int pullframe;//�͂Ȃ���Ă���t���[����
    } framestate_[static_cast<int>(ButtonsID::max)];

  public:
    //�f�t�H���g�R���X�g���N�^����
    //�R���p�C���G���[�o��Ƃ��́A=delete��{}�ɕύX��private:��
    ControllerBase() = delete;

    //�R���g���[������������
    //�@id�@�������Ώۂ̃R���g���[��ID���w��
    ControllerBase(ID id)
      :
      now_(0),
      prev_(0),
      id_(static_cast<int>(id))
    {
      for (auto& framestate : framestate_)
        framestate.holdframe = framestate.pullframe = 0;
    }

    //���g�p
    virtual void render() { }

    //���ʂ�update
    virtual void update() = 0;

    virtual void update_common()
    {
      //�e�{�^���ɑ΂��A������Ă���t���[���͂Ȃ���Ă���t���[�����J�E���g
      for (int i = 0; i < static_cast<int>(ButtonsID::max); ++i)
      {
        ButtonsID buttonID = static_cast<ButtonsID>(i);
        //���̃t���[���őΏۃ{�^���������ꂽ���͂Ȃ��ꂽ���Ńt���[���J�E���^��0�N���A
        if (checkPush(buttonID) || checkPull(buttonID))
        {
          framestate_[i].holdframe = -1;
          framestate_[i].pullframe = -1;
        }
        //������Ă��邩�͂Ȃ���Ă��邩�Ńt���[���J�E���^���C���N�������g
        if (checkPress(buttonID)) ++framestate_[i].holdframe;
        if (checkFree(buttonID))  ++framestate_[i].pullframe;
      }
    }

    //������Ă��邩�̃`�F�b�N
    virtual bool checkPress(const ButtonsID btn, int interval = 1) const
    {
      WORD flag = 1 << static_cast<int>(btn);
      return ((now_ & flag) == flag && (framestate_[static_cast<int>(btn)].holdframe % interval) == 0);
    }

    //������Ă��Ȃ����̃`�F�b�N
    virtual bool checkFree(const ButtonsID btn, int interval = 1)  const
    {
      WORD flag = 1 << static_cast<int>(btn);
      return ((now_ & flag) == 0 && (framestate_[static_cast<int>(btn)].pullframe % interval) == 0);
    }

    //�����ꂽ���̃`�F�b�N
    virtual bool checkPush(const ButtonsID btn)  const
    {
      WORD flag = 1 << static_cast<int>(btn);
      return ((prev_ & flag) == 0) && ((now_ & flag) != 0);
    }

    //�͂Ȃ��ꂽ���̃`�F�b�N
    virtual bool checkPull(const ButtonsID btn)  const
    {
      WORD flag = 1 << static_cast<int>(btn);
      return ((prev_ & flag) != 0) && ((now_ & flag) == 0);
    }

    //�g���K�[�X�e�B�b�N���̏����擾����
    //0.0~1.0�ŕԂ��Ă���
    virtual float leftTrigger() const = 0;

    //�g���K�[�X�e�B�b�N�E�̏����擾����
    //0.0~1.0�ŕԂ��Ă���
    virtual float rightTrigger() const = 0;

    //�A�i���O�X�e�B�b�N���̏����擾����
    //1.0~-1.0�ŕԂ��Ă���
    virtual D3DXVECTOR2 leftThumb() const = 0;

    //�A�i���O�X�e�B�b�N�E�̏����擾����
    //1.0~-1.0�ŕԂ��Ă���
    virtual D3DXVECTOR2 rightThumb() const = 0;

    //�p�b�h��U��������
    //�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
    //�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
    virtual void vibration(float leftpower, float rightpower) {}

    //�p�b�h��U��������
    //�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
    virtual void vibrationLeft(float leftpower) {}

    //�p�b�h��U��������
    //�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
    virtual void vibrationRight(float rightpower) {}

    //�p�b�h��U��������
    //�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
    //�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
    virtual void vibrationFrame(float leftpower, float rightpower, int frame) {}

    //�p�b�h��U��������
    //�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
    virtual void vibrationLeftFrame(float leftpower, int frame) {}

    //�p�b�h��U��������
    //�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
    virtual void vibrationRightFrame(float rightpower, int frame) {}

  };


  //--------------------------------------------------------------------------------------------
  //XInput�Ή��p�b�h����
  //--------------------------------------------------------------------------------------------
  class Controller : public ControllerBase
  {
  private:
    const float deadzone_ = static_cast<float>(0x7FFF) * 0.24f;//�j���[�g�������̂����ђl�͈̔�  microsoft�̃f�t�H���g�l��24%�w��Ȃ̂ł����Ȃ���

    XINPUT_STATE state_;//�{�^�����
    XINPUT_VIBRATION vibration_;//�Ԃ�Ԃ���
    enum Motor
    {
      left, right, motornum
    };
    std::array<int, motornum> vibrationFrame_;
    bool connected_;//�R�l�N�g����Ă��邩

    //�A�i���O�p�b�h���̎擾
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
    //�f�t�H���g�R���X�g���N�^����
    //�R���p�C���G���[�o��Ƃ��́A=delete��{}�ɕύX��private:��
    Controller() = delete;

    //�R���g���[������������
    //�@id�@�������Ώۂ̃R���g���[��ID���w��
    Controller(ID id)
      :
      ControllerBase(id),
      state_(),
      vibration_(),
      connected_(false)
    {
      vibrationFrame_.fill(-1);
    }

    //�p�b�h�pupdate
    void update() override
    {
      //�O��̃`�F�b�N
      prev_ = now_;

      //�p�b�h���������`�F�b�N
      if (XInputGetState(id_, &state_) != ERROR_SUCCESS)
      {
        connected_ = false;
        now_ = 0;
        return;
      }

      connected_ = true;

      //���݂̃{�^�����̎擾
      now_ = state_.Gamepad.wButtons;

      //�j���[�g�������̂����ђl������������i���j
      if ((state_.Gamepad.sThumbLX < deadzone_ &&
        state_.Gamepad.sThumbLX > -deadzone_) &&
        (state_.Gamepad.sThumbLY < deadzone_ &&
        state_.Gamepad.sThumbLY > -deadzone_))
      {
        state_.Gamepad.sThumbLX = 0;
        state_.Gamepad.sThumbLY = 0;
      }

      //�j���[�g�������̂����ђl������������i�E�j
      if ((state_.Gamepad.sThumbRX < deadzone_ &&
        state_.Gamepad.sThumbRX > -deadzone_) &&
        (state_.Gamepad.sThumbRY < deadzone_ &&
        state_.Gamepad.sThumbRY > -deadzone_))
      {
        state_.Gamepad.sThumbRX = 0;
        state_.Gamepad.sThumbRY = 0;
      }

      update_common();

      //���[�^�[�̒�~���s��
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

    //������Ă��邩�̃`�F�b�N
    bool checkPress(const ButtonsID btn, int interval = 1) const override
    {
      if (!connected_) return false;
      return ControllerBase::checkPress(btn, interval);
    }

    //������Ă��Ȃ����̃`�F�b�N
    bool checkFree(const ButtonsID btn, int interval = 1)  const override
    {
      if (!connected_) return false;
      return ControllerBase::checkFree(btn, interval);
    }

    //�����ꂽ���̃`�F�b�N
    bool checkPush(const ButtonsID btn)  const override
    {
      if (!connected_) return false;
      return ControllerBase::checkPush(btn);
    }

    //�͂Ȃ��ꂽ���̃`�F�b�N
    bool checkPull(const ButtonsID btn)  const override
    {
      if (!connected_) return false;
      return ControllerBase::checkPull(btn);
    }

    //�g���K�[�X�e�B�b�N���̏����擾����
    //0.0~1.0�ŕԂ��Ă���
    float leftTrigger() const override
    {
      if (!connected_) return false;
      return static_cast<float>(state_.Gamepad.bLeftTrigger) / 255.0f;
    }

    //�g���K�[�X�e�B�b�N�E�̏����擾����
    //0.0~1.0�ŕԂ��Ă���
    float rightTrigger() const override
    {
      if (!connected_) return false;
      return static_cast<float>(state_.Gamepad.bRightTrigger) / 255.0f;
    }

    //�A�i���O�X�e�B�b�N���̏����擾����
    //1.0~-1.0�ŕԂ��Ă���
    D3DXVECTOR2 leftThumb() const override
    {
      return getThumb(state_.Gamepad.sThumbLX, state_.Gamepad.sThumbLY);
    }

    //�A�i���O�X�e�B�b�N�E�̏����擾����
    //1.0~-1.0�ŕԂ��Ă���
    D3DXVECTOR2 rightThumb() const override
    {
      return getThumb(state_.Gamepad.sThumbRX, state_.Gamepad.sThumbRY);
    }

    //�p�b�h��U��������
    //�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
    //�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
    void vibration(float leftpower, float rightpower) override
    {
      vibration_.wLeftMotorSpeed = static_cast<WORD>(65535.0f * leftpower);
      vibration_.wRightMotorSpeed = static_cast<WORD>(65535.0f * rightpower);
      XInputSetState(id_, &vibration_);
    }
    //�p�b�h��U��������
    //�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
    void vibrationLeft(float leftpower) override
    {
      vibration_.wLeftMotorSpeed = static_cast<WORD>(65535.0f * leftpower);
      XInputSetState(id_, &vibration_);
    }
    //�p�b�h��U��������
    //�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
    void vibrationRight(float rightpower) override
    {
      vibration_.wRightMotorSpeed = static_cast<WORD>(65535.0f * rightpower);
      XInputSetState(id_, &vibration_);
    }

    //�p�b�h��U��������
    //�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
    //�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
    void vibrationFrame(float leftpower, float rightpower, int frame) override
    {
      vibration(leftpower, rightpower);
      vibrationFrame_[Motor::left] = frame;
      vibrationFrame_[Motor::right] = frame;
    }
    //�p�b�h��U��������
    //�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
    void vibrationLeftFrame(float leftpower, int frame) override
    {
      vibrationLeft(leftpower);
      vibrationFrame_[Motor::left] = frame;
    }
    //�p�b�h��U��������
    //�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
    void vibrationRightFrame(float rightpower, int frame) override
    {
      vibrationRight(rightpower);
      vibrationFrame_[Motor::right] = frame;
    }
  };

  //--------------------------------------------------------------------------------------------
  //�L�[�{�[�h����
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
    //�f�t�H���g�R���X�g���N�^����
    //�R���p�C���G���[�o��Ƃ��́A=delete��{}�ɕύX��private:��
    KeyboardController() = delete;

    //�R���g���[������������
    //�@id�@�������Ώۂ̃R���g���[��ID���w��
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
    
    //�L�[�{�[�h�pupdate
    void update() override
    {
      //�O��̃`�F�b�N
      prev_ = now_;

      //�e�{�^���̏�Ԃ��X�V
      now_ = 0;
      for (int i = 0; i < static_cast<int>(ButtonsID::max); ++i)
      {
        WORD flag = (1 << i);
        if (GetAsyncKeyState(keymap_[i]))
          now_ |= flag;
      }

      update_common();
    }

    //�g���K�[�X�e�B�b�N���̏����擾����
    //0.0~1.0�ŕԂ��Ă���
    float leftTrigger() const override
    {
      return 0;
    }

    //�g���K�[�X�e�B�b�N�E�̏����擾����
    //0.0~1.0�ŕԂ��Ă���
    float rightTrigger() const override
    {
      return 0;
    }

    //�A�i���O�X�e�B�b�N���̏����擾����
    //1.0~-1.0�ŕԂ��Ă���
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

    //�A�i���O�X�e�B�b�N�E�̏����擾����
    //1.0~-1.0�ŕԂ��Ă���
    D3DXVECTOR2 rightThumb() const override
    {
      return D3DXVECTOR2(0, 0);
    }
  };

private:

  std::vector<std::unique_ptr<ControllerBase>> controllers_;

  //�e��R���X�g���N�^����
  //�֐��w���p�[�̂��߂ɃV���O���g�����f���ɂ�������
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
  //�C���X�^���X�擾
  static XInput* GetInstance()
  {
    static XInput instance;
    return &instance;
  }

  //���t���[���Ă�
  void update()
  {
    for (auto& controller : controllers_)
      controller->update();
  }

  //�`��i�Ȃɂ����Ȃ��j
  void render()
  {
    for (auto& controller : controllers_)
      controller->render();
  }

  //������Ă��邩�̃`�F�b�N
  bool checkPress(const ButtonsID btn, int interval = 1, ID id = ID::controller_1P) const
  {
    return controllers_[static_cast<int>(id)]->checkPress(btn, interval);
  }

  //������Ă��Ȃ����̃`�F�b�N
  bool checkFree(const ButtonsID btn, int interval = 1, ID id = ID::controller_1P)  const
  {
    return controllers_[static_cast<int>(id)]->checkFree(btn, interval);
  }

  //�����ꂽ���̃`�F�b�N
  bool checkPush(const ButtonsID btn, ID id = ID::controller_1P)  const
  {
    return controllers_[static_cast<int>(id)]->checkPush(btn);
  }

  //�͂Ȃ��ꂽ���̃`�F�b�N
  bool checkPull(const ButtonsID btn, ID id = ID::controller_1P)  const
  {
    return controllers_[static_cast<int>(id)]->checkPull(btn);
  }

  //1P~4P�܂ł̂ǂꂩ�̃{�^����������Ă��邩���`�F�b�N
  bool checkAnyPress(const ButtonsID btn) const
  {
    for (const auto& controller : controllers_)
      if (controller->checkPress(btn, 1)) return true;
    return false;
  }

  //1P~4P�܂ł̂ǂꂩ�̃{�^���������ꂽ�����`�F�b�N
  bool checkAnyPush(const ButtonsID btn) const
  {
    for (const auto& controller : controllers_)
      if (controller->checkPush(btn)) return true;
    return false;
  }

  //1P~4P�܂ł̂ǂꂩ�̃{�^�����͂Ȃ���Ă��邩���`�F�b�N
  bool checkAnyFree(const ButtonsID btn) const
  {
    for (const auto& controller : controllers_)
      if (controller->checkFree(btn, 1)) return true;
    return false;
  }

  //1P~4P�܂ł̂ǂꂩ�̃{�^�����͂Ȃ��ꂽ�����`�F�b�N
  bool checkAnyPull(const ButtonsID btn) const
  {
    for (const auto& controller : controllers_)
      if (controller->checkPull(btn)) return true;
    return false;
  }

  //�g���K�[�X�e�B�b�N���̏����擾����
  //0.0~1.0�ŕԂ��Ă���
  float leftTrigger(ID id = ID::controller_1P) const
  {
    return controllers_[static_cast<int>(id)]->leftTrigger();
  }

  //�g���K�[�X�e�B�b�N�E�̏����擾����
  //0.0~1.0�ŕԂ��Ă���
  float rightTrigger(ID id = ID::controller_1P) const
  {
    return controllers_[static_cast<int>(id)]->rightTrigger();
  }

  //�A�i���O�X�e�B�b�N���̏����擾����
  //1.0~-1.0�ŕԂ��Ă���
  D3DXVECTOR2 leftThumb(ID id = ID::controller_1P) const
  {
    return controllers_[static_cast<int>(id)]->leftThumb();
  }

  //�A�i���O�X�e�B�b�N�E�̏����擾����
  //1.0~-1.0�ŕԂ��Ă���
  D3DXVECTOR2 rightThumb(ID id = ID::controller_1P) const
  {
    return controllers_[static_cast<int>(id)]->rightThumb();
  }

  //�p�b�h��U��������
  //�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  //�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  void vibration(float leftpower, float rightpower, ID id = ID::controller_1P)
  {
    controllers_[static_cast<int>(id)]->vibration(leftpower, rightpower);
  }

  //�p�b�h��U��������
  //�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  void vibrationLeft(float leftpower, ID id = ID::controller_1P)
  {
    controllers_[static_cast<int>(id)]->vibrationLeft(leftpower);
  }

  //�p�b�h��U��������
  //�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  void vibrationRight(float rightpower, ID id = ID::controller_1P)
  {
    controllers_[static_cast<int>(id)]->vibrationRight(rightpower);
  }

  //�p�b�h��U��������
  //�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  //�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  void vibrationFrame(float leftpower, float rightpower, int frame, ID id = ID::controller_1P)
  {
    controllers_[static_cast<int>(id)]->vibrationFrame(leftpower, rightpower, frame);
  }

  //�p�b�h��U��������
  //�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  void vibrationLeftFrame(float leftpower, int frame, ID id = ID::controller_1P)
  {
    controllers_[static_cast<int>(id)]->vibrationLeftFrame(leftpower, frame);
  }

  //�p�b�h��U��������
  //�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  void vibrationRightFrame(float rightpower, int frame, ID id = ID::controller_1P)
  {
    controllers_[static_cast<int>(id)]->vibrationRightFrame(rightpower, frame);
  }

  //----------------------------------------------------------------------------------------
  //�֐��̂悤�Ɏg�p���邽�߂̃��b�p�֐��S
  //���t���[���Ăт܂��傤
  static void CheckKey()
  {
    XInput::GetInstance()->update();
  }

  //������Ă��邩�`�F�b�N
  static bool CheckPress(const XInput::ButtonsID buttonID, int interval = 1, XInput::ID id = XInput::ID::controller_1P)
  {
    return XInput::GetInstance()->checkPress(buttonID, interval, id);
  }

  //�͂Ȃ���Ă��邩�`�F�b�N
  static bool CheckFree(const XInput::ButtonsID buttonID, int interval = 1, XInput::ID id = XInput::ID::controller_1P)
  {
    return XInput::GetInstance()->checkFree(buttonID, interval, id);
  }

  //�����ꂽ���̃`�F�b�N
  static bool CheckPush(const XInput::ButtonsID btn, XInput::ID id = XInput::ID::controller_1P)
  {
    return XInput::GetInstance()->checkPush(btn);
  }

  //�͂Ȃ��ꂽ���̃`�F�b�N
  static bool CheckPull(const XInput::ButtonsID btn, XInput::ID id = XInput::ID::controller_1P)
  {
    return XInput::GetInstance()->checkPull(btn, id);
  }

  //1P~4P�܂ł̂ǂꂩ�̃{�^����������Ă��邩���`�F�b�N
  static bool CheckAnyPress(const XInput::ButtonsID btn)
  {
    return XInput::GetInstance()->checkAnyPress(btn);
  }

  //1P~4P�܂ł̂ǂꂩ�̃{�^����������Ă��邩���`�F�b�N
  static bool CheckAnyPush(const XInput::ButtonsID btn)
  {
    return XInput::GetInstance()->checkAnyPush(btn);
  }

  //1P~4P�܂ł̂ǂꂩ�̃{�^�����͂Ȃ���Ă��邩���`�F�b�N
  static bool CheckAnyFree(const XInput::ButtonsID btn)
  {
    return XInput::GetInstance()->checkAnyFree(btn);
  }

  //1P~4P�܂ł̂ǂꂩ�̃{�^�����͂Ȃ��ꂽ�����`�F�b�N
  static bool CheckAnyPull(const XInput::ButtonsID btn)
  {
    return XInput::GetInstance()->checkAnyPull(btn);
  }

  //�g���K�[�X�e�B�b�N���̏����擾����
  //0.0~1.0�ŕԂ��Ă���
  static float LeftTrigger(XInput::ID id = XInput::ID::controller_1P)
  {
    return XInput::GetInstance()->leftTrigger(id);
  }

  //�g���K�[�X�e�B�b�N�E�̏����擾����
  //0.0~1.0�ŕԂ��Ă���
  static float RightTrigger(XInput::ID id = XInput::ID::controller_1P)
  {
    return XInput::GetInstance()->rightTrigger(id);
  }

  //�A�i���O�X�e�B�b�N���̏����擾����
  //1.0~-1.0�ŕԂ��Ă���
  static D3DXVECTOR2 LeftThumb(XInput::ID id = XInput::ID::controller_1P)
  {
    return XInput::GetInstance()->leftThumb(id);
  }

  //�A�i���O�X�e�B�b�N�E�̏����擾����
  //1.0~-1.0�ŕԂ��Ă���
  static D3DXVECTOR2 RightThumb(XInput::ID id = XInput::ID::controller_1P)
  {
    return XInput::GetInstance()->rightThumb(id);
  }

  //�p�b�h��U��������
  //�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  //�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  static void Vibration(float leftpower, float rightpower, XInput::ID id = XInput::ID::controller_1P)
  {
    XInput::GetInstance()->vibration(leftpower, rightpower, id);
  }

  //�p�b�h��U��������
  //�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  static void VibrationLeft(float leftpower, XInput::ID id = XInput::ID::controller_1P)
  {
    XInput::GetInstance()->vibrationLeft(leftpower, id);
  }

  //�p�b�h��U��������
  //�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  static void VibrationRight(float rightpower, XInput::ID id = XInput::ID::controller_1P)
  {
    XInput::GetInstance()->vibrationRight(rightpower, id);
  }

  //�p�b�h���w��t���[�����U��������
  //�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  //�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  static void VibrationFrame(float leftpower, float rightpower, int frame, XInput::ID id = XInput::ID::controller_1P)
  {
    XInput::GetInstance()->vibrationFrame(leftpower, rightpower, frame, id);
  }

  //�p�b�h��U��������
  //�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  static void VibrationLeftFrame(float leftpower, int frame, XInput::ID id = XInput::ID::controller_1P)
  {
    XInput::GetInstance()->vibrationLeftFrame(leftpower, frame, id);
  }

  //�p�b�h��U��������
  //�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  static void VibrationRightFrame(float rightpower, int frame, XInput::ID id = XInput::ID::controller_1P)
  {
    XInput::GetInstance()->vibrationRightFrame(rightpower, frame, id);
  }
};


}