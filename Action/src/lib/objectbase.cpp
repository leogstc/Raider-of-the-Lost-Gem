

#include "gplib.h"
#include "objectbase.h"

#include "ci_ext/vec3.hpp"
#include "ci_ext/Polygon.hpp"
using namespace objectbase;
using namespace gplib;
namespace objectbase
{
  Vec3f::Vec3f(){
    x = 0.f; y = 0.f; z = 0.f;
  }
  Vec3f::Vec3f(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
  }
  BaseData::BaseData(){}
  BaseData::BaseData(int w, int h, const std::string resname)
  {
    wid = w;
    hgt = h;
    this->resname = resname;
  }

  //------------------------------------------------------
  //CharBase���p���ėp����
  //----------------------
  //�A�j���[�V�����ݒ�
  //max : �O�ɖ߂閇��
  //speed : �ؑւ܂ł̃t���[��
  void Anim::set(int max, int speed)
  {
    cnt_ = 0.f;
    max_ = max;
    speed_ = speed;
    no_ = 0;
    flag_ = false;
  }

  //�A�j���X�V
  //���̊֐���no�̒l���X�V����Bno�̒l��
  //src.x�ɑ������B
  //�A�j���̏�����ύX�������ꍇ�͈ȉ�
  bool Anim::step()
  {
    if (!flag_)  return false;
    cnt_++;
    if (cnt_ >= speed_){
      cnt_ = 0;
      no_++;
      if (no_ >= max_){
        no_ = 0;
      }
      return true;
    }
    return false;
  }

  //----------------------
  //�A�j���[�V������~
  //speed��0�ɂ��邾���Ȃ̂ŁA�ĊJ���͍ēxSetAnim���K�v
  void Anim::stop()
  {
    flag_ = false;
  }

  void Anim::resume()
  {
    flag_ = true;
  }
  //�A�j���[�V�����J�n
  void Anim::start()
  {
    flag_ = true;
    cnt_ = 0.f;
    no_ = 0;
  }

  BoxData::BoxData(int x, int y, int w, int h) :
    x_(x),
    y_(y),
    w_(w),
    h_(h)
  {
    rt_.left = x;
    rt_.right = x + w;
    rt_.top = y;
    rt_.bottom = y + h;

    naka_color_ = ARGB(255, 0, 0, 0);
  }
  void BoxData::draw()
  {
    draw::Box(x_, y_, x_ + w_, y_ + h_, 0.f, naka_color_, ARGB(255, 255, 0, 0), 1, 1);

  }
  void BoxData::changeNakaColor(int alpha, int red, int green, int blue)
  {
    naka_color_ = ((alpha & 0xff) << 24) + ((red & 0xff) << 16) + ((green & 0xff) << 8) + blue;
  }

  //------------------------------------------------------
  //CharBase����������
  //
  /*
  //��{�ƂȂ�`�b�v�̃T�C�Y�Ɨ��p����摜
  BaseData base = {CHARWID,CHARHGT,charIMG};
  //�������@�w��O�̒l�͏����l�ɂȂ�B
  //���̏ꍇ��wid,hgt,img�ȊO�͏����l
  InitCharBase(player.pos,base);

  //���̏ꍇ��wid,hgt,img,x,y�ȊO�͏����l
  InitCharBase(player.pos,base,STARTPOSX,STARTPOSY);

  ���p����ۂ͉��t���Oshow���U�ɂȂ��Ă��邽�߁A
  UseChar�ŉ���Ԃɂ��邱�ƁB
  */
  void ObjData::init(const BaseData& base,
    float x, float y, float z, float addx, float addy, float addz,
    int srcX, int srcY, int srcZ, float angle, float scaleX, float scaleY, float scaleZ,
    float alpha, float red, float green, float blue)
  {
    width = base.wid;
    height = base.hgt;
    resname = base.resname;

    pos.x = x;
    pos.y = y;
    pos.z = z;
    add.x = addx;
    add.y = addy;
    add.z = addz;
    scale.x = scaleX;
    scale.y = scaleY;
    scale.z = scaleZ;
    this->angle = angle;
    src.x = (float)srcX;
    src.y = (float)srcY;
    src.z = (float)srcZ;

    a = alpha;
    r = red;
    g = green;
    b = blue;

    show = false;
  }

  void ObjData::init(float x, float y, float z, float addx, float addy, float addz,
    const std::string& srcResName, int srcX, int srcY, int srcZ, int w, int h,
    float angle, float scaleX, float scaleY, float scaleZ,
    float alpha, float red, float green, float blue)
  {
    pos.x = x;
    pos.y = y;
    pos.z = z;
    add.x = addx;
    add.y = addy;
    add.z = addz;
    scale.x = scaleX;
    scale.y = scaleY;
    scale.z = scaleZ;
    src.x = (float)srcX;
    src.y = (float)srcY;
    src.z = (float)srcZ;
    resname = srcResName;
    width = w;
    height = h;
    this->angle = angle;
    a = alpha;
    r = red;
    g = green;
    b = blue;
    show = false;

  }

  //------------------------------------------------------
  //CharBase����������
  //���ׂĂO�ɂ���B
  void ObjData::init()
  {
    init(0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
      "", 0, 0, 0, 0, 0,
      0.f, 0.f, 0.f, 0.f,
      0.f, 0.f, 0.f, 0.f);
  }

  //���p�`�F�b�N
  bool ObjData::checkUse() const
  {
    return (show == true);
  }
  bool ObjData::getUse() const { return show; }



  //�ėp�ړ�
  void ObjData::move()
  {
    pos.x += add.x;
    pos.y += add.y;
    pos.z += add.z;
  }

  //----------------------
  //�ėp��������
  void ObjData::setUseOff()
  {
    show = false;
  }

  //----------------------
  //�ėp�o������
  void ObjData::setUseOn()
  {
    show = true;
  }

  void ObjData::set(bool flag)
  {
    show = flag;
  }

  //----------------------
  //����p��`�쐬 �������W��@�g�嗦�l��
  //�傫�����N���b�s���O�T�C�Y�ƈقȂ�ꍇ�ɗ��p����B
  //startx,starty : ����_����̃I�t�Z�b�g�l�A�l�����ォ����Z�����
  //endx,endy : �E���_����̃I�t�Z�b�g�l�A�l���E�����猸�Z�����
  void ObjData::makeRectScales(RECT &rt, int startx, int starty, int endx, int endy) const
  {
    SetRect(&rt, (int)pos.x - (int)((width*scale.x) / 2) + startx, (int)pos.y - (int)((height * scale.y) / 2) + starty,
      (int)pos.x + (int)((width*scale.x) / 2) - endx, (int)pos.y + (int)((height * scale.y) / 2) - endy);
  }

  RECT ObjData::makeRectScales(int startx, int starty, int endx, int endy) const
  {
    RECT rt;
    SetRect(&rt, (int)pos.x - (int)((width*scale.x) / 2) + startx, (int)pos.y - (int)((height * scale.y) / 2) + starty,
      (int)pos.x + (int)((width*scale.x) / 2) - endx, (int)pos.y + (int)((height * scale.y) / 2) - endy);
    return rt;
  }

  //----------------------
  //����p��`�쐬 ������W�
  //�傫�����N���b�s���O�T�C�Y�ƈقȂ�ꍇ�ɗ��p����B
  //startx,starty : ����_����̃I�t�Z�b�g�l�A�l�����ォ����Z�����
  //endx,endy : �E���_����̃I�t�Z�b�g�l�A�l���E�����猸�Z�����
  void ObjData::makeRectLeftTop(RECT &rt, int startx, int starty, int endx, int endy) const
  {
    SetRect(&rt, (int)pos.x + startx, (int)pos.y + starty,
      (int)pos.x + width - endx, (int)pos.y + height - endy);
  }

  RECT ObjData::makeRectLeftTop(int startx, int starty, int endx, int endy) const
  {
    RECT rt;
    SetRect(&rt, (int)pos.x + startx, (int)pos.y + starty,
      (int)pos.x + width - endx, (int)pos.y + height - endy);
    return rt;
  }

  //----------------------
  //����p��`�쐬 �������W�
  //�傫�����N���b�s���O�T�C�Y�ƈقȂ�ꍇ�ɗ��p����B
  //startx,starty : ����_����̃I�t�Z�b�g�l�A�l�����ォ����Z�����
  //endx,endy : �E���_����̃I�t�Z�b�g�l�A�l���E�����猸�Z�����
  void ObjData::makeRect(RECT &rt, int startx, int starty, int endx, int endy) const
  {
    SetRect(&rt, (int)pos.x - width / 2 + startx, (int)pos.y - height / 2 + starty,
      (int)pos.x + width / 2 - endx, (int)pos.y + height / 2 - endy);
  }
  RECT ObjData::makeRect(int startx, int starty, int endx, int endy) const
  {
    RECT rt;
    SetRect(&rt, (int)pos.x - width / 2 + startx, (int)pos.y - height / 2 + starty,
      (int)pos.x + width / 2 - endx, (int)pos.y + height / 2 - endy);
    return rt;
  }


  //----------------------
  //�ėp�`�揈�� �����
  void ObjData::draw() const
  {
    draw::Graph(
      pos.x, pos.y, pos.z,
      resname,
      width	* (int)(src.x),
      height * (int)(src.y),
      width, height,
      (int)angle,
      scale.x, scale.y,
      (u_char)a, (u_char)r, (u_char)g, (u_char)b);
  }

  //----------------------
  //�ėp�`�揈�� ����
  void ObjData::draw2() const
  {
    draw::GraphLeftTop(
      pos.x, pos.y, pos.z,
      resname,
      width	* (int)(src.x),
      height * (int)(src.y),
      width, height,
      (int)angle, nullptr,
      scale.x, scale.y,
      (u_char)a, (u_char)r, (u_char)g, (u_char)b);
  }


  //----------------------
  //�ėp�`�揈�� �����
  void ObjData::drawNC() const
  {

    draw::GraphNC(
      pos.x, pos.y, pos.z,
      resname,
      width	* (int)(src.x),
      height * (int)(src.y),
      width, height,
      (int)angle,
      scale.x, scale.y,
      (u_char)a, (u_char)r, (u_char)g, (u_char)b);
  }

  //----------------------
  //�ėp�`�揈�� ����
  void ObjData::draw2NC() const
  {
    draw::GraphLeftTopNC(
      pos.x, pos.y, pos.z,
      resname,
      width	* (int)(src.x),
      height * (int)(src.y),
      width, height,
      (int)angle, nullptr,
      scale.x, scale.y,
      (u_char)a, (u_char)r, (u_char)g, (u_char)b);
  }
  //----------------------
  //������`�F�b�N
  bool ObjData::checkHit(const RECT& destRt) const
  {
    RECT rt;
    makeRect(rt);
    //�f�o�b�O----------------
    //draw::CkRect(destRt);
    //draw::CkRect(rt);
    //graph::Draw_CkRect(destRt);
    //graph::Draw_CkRect(rt);
    //�����܂�----------------
    if (math::RectCheck(rt, destRt)){
      return true;
    }
    else{
      return false;
    }
  }

  //----------------------
  //������`�F�b�N �g�嗦�l��
  bool ObjData::checkHitScales(const RECT& destRt) const
  {
    RECT rt;
    makeRectScales(rt);
    //�f�o�b�O----------------
    //	Draw_CkRect(rt1);
    //	Draw_CkRect(rt2);
    //�����܂�----------------
    if (math::RectCheck(rt, destRt)){
      return true;
    }
    else{
      return false;
    }
  }
  //----------------------
  //������`�F�b�N
  bool ObjData::checkHit(ObjData& destObj) const
  {
    RECT srcRt;
    makeRect(srcRt);

    RECT destRt = destObj.makeRect();
    //�f�o�b�O----------------
    //draw::CkRect(destRt);
    //draw::CkRect(srcRt);
    //graph::Draw_CkRect(destRt);
    //graph::Draw_CkRect(rt);
    //�����܂�----------------
    if (math::RectCheck(srcRt, destRt)){
      return true;
    }
    else{
      return false;
    }
  }

  //----------------------
  //������`�F�b�N �g�嗦�l��
  bool ObjData::checkHitScales(ObjData& destObj) const
  {
    RECT srcRt;
    makeRectScales(srcRt);

    RECT destRt;
    destObj.makeRectScales(destRt);
    //�f�o�b�O----------------
    //	Draw_CkRect(rt1);
    //	Draw_CkRect(rt2);
    //�����܂�----------------
    if (math::RectCheck(srcRt, destRt)){
      return true;
    }
    else{
      return false;
    }
  }
  //----------------------
  //�g�`��
  //  cb_ charbase���
  //  color �F���iARGB�ł��w��\�j
  //  lineweight ���C���̑���
  void ObjData::Draw_Frame(ci_ext::Color color, int lineweight) const
  {
    ci_ext::Polygon<float> a(
      ci_ext::Vec3<float>(pos.x, pos.y, pos.z), //pos
      {
        ci_ext::Vec3<float>(-width / 2.0f, -height / 2.0f, 0.0f),
        ci_ext::Vec3<float>(-width / 2.0f, height / 2.0f, 0.0f),
        ci_ext::Vec3<float>(width / 2.0f, height / 2.0f, 0.0f),
        ci_ext::Vec3<float>(width / 2.0f, -height / 2.0f, 0.0f),
      },//vertex
      angle,//degree
      ci_ext::Vec3<float>(scale.x, scale.y, 1.0f)//scale
      );

    a.draw(color, lineweight);
  }
  //----------------------
  //������`�F�b�N
  bool ObjData::checkHit2(const ObjData& cb, bool debug_mode) const
  {
    //�|���S������Ĕ��肳����
    ci_ext::Polygon<float> a(
      ci_ext::Vec3<float>(pos.x, pos.y, pos.z), //pos
      {
        ci_ext::Vec3<float>(-width / 2.0f, -height / 2.0f, 0.0f),
        ci_ext::Vec3<float>(-width / 2.0f, height / 2.0f, 0.0f),
        ci_ext::Vec3<float>(width / 2.0f, height / 2.0f, 0.0f),
        ci_ext::Vec3<float>(width / 2.0f, -height / 2.0f, 0.0f),
      },//vertex
      angle,//degree
      ci_ext::Vec3<float>(scale.x, scale.y, 1.0f)//scale
      );

    ci_ext::Polygon<float> b(
      ci_ext::Vec3<float>(cb.pos.x, cb.pos.y, cb.pos.z),
      {
        ci_ext::Vec3<float>(-cb.width / 2.0f, -cb.height / 2.0f, 0.0f),
        ci_ext::Vec3<float>(-cb.width / 2.0f, cb.height / 2.0f, 0.0f),
        ci_ext::Vec3<float>(cb.width / 2.0f, cb.height / 2.0f, 0.0f),
        ci_ext::Vec3<float>(cb.width / 2.0f, -cb.height / 2.0f, 0.0f),
      },
      cb.angle,
      ci_ext::Vec3<float>(cb.scale.x, cb.scale.y, 1.0f)
      );

    if (debug_mode)
    {
      Draw_Frame(0xFFFF0000, 1);
      cb.Draw_Frame(0xFFFF0000, 1);
    }
    return a.intersect(b);
  }
  /*
    //----------------------
    //������`�F�b�N
    bool ObjData::HitCheck2(const ObjData& cb1, const ObjData& cb2)
    {
    std::vector<ci_ext::Vec3<float>> v1 = {
    ci_ext::Vec3<float>(-cb1.width / 2.0f, -cb1.height / 2.0f, 0.0f),
    ci_ext::Vec3<float>(-cb1.width / 2.0f, cb1.height / 2.0f, 0.0f),
    ci_ext::Vec3<float>(cb1.width / 2.0f, cb1.height / 2.0f, 0.0f),
    ci_ext::Vec3<float>(cb1.width / 2.0f, -cb1.height / 2.0f, 0.0f),
    };
    std::vector<ci_ext::Vec3<float>> v2 = {
    ci_ext::Vec3<float>(-cb2.width / 2.0f, -cb2.height / 2.0f, 0.0f),
    ci_ext::Vec3<float>(-cb2.width / 2.0f, cb2.height / 2.0f, 0.0f),
    ci_ext::Vec3<float>(cb2.width / 2.0f, cb2.height / 2.0f, 0.0f),
    ci_ext::Vec3<float>(cb2.width / 2.0f, -cb2.height / 2.0f, 0.0f),
    };
    //�|���S������Ĕ��肳����
    ci_ext::Polygon<float> a(
    ci_ext::Vec3<float>(cb1.pos.x, cb1.pos.y, cb1.pos.z),
    v1,//vertex
    cb1.angle,
    ci_ext::Vec3<float>(cb1.scale.x, cb1.scale.y, 1.0f));//scale
    ci_ext::Polygon<float> b(
    ci_ext::Vec3<float>(cb2.pos.x, cb2.pos.y, cb2.pos.z),
    v2,
    cb2.angle,
    ci_ext::Vec3<float>(cb2.scale.x, cb2.scale.y, 1.0f)
    );
    if (a.intersect(b))
    {
    return true;
    }
    return false;
    }

    //----------------------
    //�g�`��
    //  cb_ charbase���
    //  color �F���iARGB�ł��w��\�j
    //  lineweight ���C���̑���
    void ObjData::Draw_Frame(const ObjData& cb, ci_ext::Color color, int lineweight)
    {
    std::vector<ci_ext::Vec3<float>> v1 = {
    ci_ext::Vec3<float>(-cb.width / 2.0f, -cb.height / 2.0f, 0.0f),
    ci_ext::Vec3<float>(-cb.width / 2.0f, cb.height / 2.0f, 0.0f),
    ci_ext::Vec3<float>(cb.width / 2.0f, cb.height / 2.0f, 0.0f),
    ci_ext::Vec3<float>(cb.width / 2.0f, -cb.height / 2.0f, 0.0f),
    };
    ci_ext::Polygon<float> a(
    ci_ext::Vec3<float>(cb.pos.x, cb.pos.y, cb.pos.z), //pos
    v1,//vertex
    cb.angle,//degree
    ci_ext::Vec3<float>(cb.scale.x, cb.scale.y, 1.0f)//scale
    );

    a.draw(color, lineweight);
    }
    */
}
//�����܂�
//------------------------------------------------------
