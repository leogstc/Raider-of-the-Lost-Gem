#pragma once
#include "ci_ext\color.hpp"
namespace objectbase{

	//����\�ȃx�[�X�f�[�^
	//�����l��0,1.0f�̂��̂͏��O����B
	struct BaseData{
		int		wid, hgt;		//�摜������
		std::string 	resname;		//�g�p�C���[�W��
		BaseData();
    BaseData(int, int, const std::string);
	};

	struct Vec3f{
		float x, y, z;
		Vec3f();
		Vec3f(float,float,float);
	};

	//
	//
	//
	//
	class Anim{
	private:
		bool  flag_;
		float	cnt_;				//�A�j���X�V�J�E���^
	public:
		int		no_;				//���݂̃A�j��NO�@���̒l��0,1,2,3,4,�ƃJ�E���g���邾��
		int	  speed_;				//�A�j���X�V���x
		int		max_;				//�A�j�������@���̒l��no�������0����J�E���g
		//----------------------
		//�A�j���[�V�����ݒ�
		//max : �O�ɖ߂閇��
		//speed : �A�j�����x
		void set(int max, int speed);
		//�A�j���X�V
		bool step();
		//�A�j���[�V������~
		void stop();
		//�A�j���[�V�����J�n 0����
		void start();
		//�A�j���[�V�����ĊJ
		void resume();
	};

	//��`�`��̔ėp����
	class BoxData
	{
	public:
		int x_, y_, h_, w_;
    RECT rt_;
		D3DCOLOR naka_color_;
    BoxData(int x, int y, int w, int h);
    void draw();
    void changeNakaColor(int alpha, int red, int green, int blue);
	};


	class ObjData
	{
	//�{���ȉ������o��GETSET�ł̑���������́A�G��Ȃ��\���Ƃ��ׂ������A
	//���ׂẴ����o�ɑ΂��ĊO��������𔺂����������\�������邽��
	//public�œ��������B
	//�I�u�W�F�N�g�Ǘ��p�̃f�[�^�S+�����̏������\�b�h�̒񋟂��s���N���X�ł���
	public:
		Vec3f		pos;				//���W
		Vec3f		add;				//�ړ���
		int			width, height;		//�摜������

		Vec3f		scale;				//�g�嗦
		Vec3f		src;				//���o�ʒu
		std::string resname;			//�g�p�C���[�W��

		bool		show;				//���p�t���O

		float		angle;				//�p�x
		float		a;					//�A���t�@
		float		r, g, b;			//�F
		//----------------------
		//�ėp�ړ�����
		void move();
		//----------------------
		//�ėp�����ʏ���
    bool getUse() const;
    bool checkUse() const;
		//----------------------
		//�ėp��������
		void setUseOff();
		//----------------------
		//�ėp�o������
		void setUseOn();
    //----------------------
		//�ėp�o����������
		void set(bool);
		//----------------------
		//����p��`�쐬 ������W�
		//�傫�����N���b�s���O�T�C�Y�ƈقȂ�ꍇ�ɗ��p����B
		//startx,starty : ����_����̃I�t�Z�b�g�l�A�l�����ォ����Z�����
		//endx,endy : �E���_����̃I�t�Z�b�g�l�A�l���E�����猸�Z�����
		void makeRectLeftTop(RECT &rt, int startx = 0, int starty = 0, int endx = 0, int endy = 0)const;
		//----------------------
		//����p��`�쐬 �������W�
		//�傫�����N���b�s���O�T�C�Y�ƈقȂ�ꍇ�ɗ��p����B
		//startx,starty : ����_����̃I�t�Z�b�g�l�A�l�����ォ����Z�����
		//endx,endy : �E���_����̃I�t�Z�b�g�l�A�l���E�����猸�Z�����
		void makeRect(RECT &rt, int startx = 0, int starty = 0, int endx = 0, int endy = 0)const;
		//----------------------
		//����p��`�쐬 �������W��@�g�嗦�l��
		//�傫�����N���b�s���O�T�C�Y�ƈقȂ�ꍇ�ɗ��p����B
		//startx,starty : ����_����̃I�t�Z�b�g�l�A�l�����ォ����Z�����
		//endx,endy : �E���_����̃I�t�Z�b�g�l�A�l���E�����猸�Z�����
		void makeRectScales(RECT &rt, int startx = 0, int starty = 0, int endx = 0, int endy = 0)const;
    //----------------------
    //����p��`�쐬 ������W�
    //�傫�����N���b�s���O�T�C�Y�ƈقȂ�ꍇ�ɗ��p����B
    //startx,starty : ����_����̃I�t�Z�b�g�l�A�l�����ォ����Z�����
    //endx,endy : �E���_����̃I�t�Z�b�g�l�A�l���E�����猸�Z�����
		RECT makeRectLeftTop(int startx = 0, int starty = 0, int endx = 0, int endy = 0)const;
    //----------------------
    //����p��`�쐬 �������W�
    //�傫�����N���b�s���O�T�C�Y�ƈقȂ�ꍇ�ɗ��p����B
    //startx,starty : ����_����̃I�t�Z�b�g�l�A�l�����ォ����Z�����
    //endx,endy : �E���_����̃I�t�Z�b�g�l�A�l���E�����猸�Z�����
    RECT makeRect( int startx = 0, int starty = 0, int endx = 0, int endy = 0) const;
    //----------------------
    //����p��`�쐬 �������W��@�g�嗦�l��
    //�傫�����N���b�s���O�T�C�Y�ƈقȂ�ꍇ�ɗ��p����B
    //startx,starty : ����_����̃I�t�Z�b�g�l�A�l�����ォ����Z�����
    //endx,endy : �E���_����̃I�t�Z�b�g�l�A�l���E�����猸�Z�����
		RECT makeRectScales(int startx = 0, int starty = 0, int endx = 0, int endy = 0) const;
    //----------------------
		//�ėp�`�揈�� �����
    void draw() const;
		//----------------------
		//�ėp�`�揈�� ����
    void draw2() const;
		//----------------------
		//�ėp�`�揈�� ����� �J�����T�O����
    void drawNC() const;
		//----------------------
		//�ėp�`�揈�� ���� �J�����T�O����
    void draw2NC() const;
		//----------------------
		//������`�F�b�N ������RECT�𐶐������ׂ�
		bool checkHit(const RECT& destRt)const;
		//----------------------
		//������`�F�b�N ������RECT�𐶐������ׂ�B�g�嗦���l���ɓ����
		bool checkHitScales(const RECT& destRt)const;
    //----------------------
    //������`�F�b�N ������RECT�𐶐������ׂ�
		bool checkHit(ObjData& destObj)const;
    //----------------------
    //������`�F�b�N ������RECT�𐶐������ׂ�B�g�嗦���l���ɓ����
		bool checkHitScales(ObjData& destObj)const;

    bool checkHit2(const ObjData& cb, bool debug_mode = false) const;
    void Draw_Frame(ci_ext::Color color, int lineweight) const;
      //------------------------------------------------------
		//����������
		/*
		//��{�ƂȂ�`�b�v�̃T�C�Y�Ɨ��p����摜
		BaseData base = {CHARWID,CHARHGT,charIMG};
		//�������@�w��O�̒l�͏����l�ɂȂ�B
		//���̏ꍇ��wid,hgt,img�ȊO�͏����l
		Init(player.pos,base);

		//���̏ꍇ��wid,hgt,img,x,y�ȊO�͏����l
		Init(player.pos,base,STARTPOSX,STARTPOSY);

		���p����ۂ͉��t���Oshow���U�ɂȂ��Ă��邽�߁A
		Use�ŉ���Ԃɂ��邱�ƁB
		*/
		void init(const BaseData& base,
			float x = 0.f, float y = 0.f, float z = 0.f, float addx = 0.f, float addy = 0.f, float addz = 0.f,
			int srcX = 0, int srcY = 0, int srcZ = 0, float angle = 0.f, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.f,
			float alpha = 255.f, float red = 255.f, float green = 255.f, float blue = 255.f);

		//------------------------------------------------------
		//����������
		//
		/*
		//�������@�w��O�̒l�͏����l�ɂȂ�B
		���p����ۂ͉��t���Oshow���U�ɂȂ��Ă��邽�߁A
		Use�ŉ���Ԃɂ��邱�ƁB
		*/
		void init(float x, float y, float z, float addx, float addy, float addz,
			const std::string& resname, int srcX, int srcY, int srcZ, int w, int h,
			float angle = 0.f, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f,
			float alpha = 255.f, float red = 255.f, float green = 255.f, float blue = 255.f);

		//------------------------------------------------------
		//����������
		//�����\���̂��O�N���A����
		void init();
		ObjData(){};
		ObjData(float x, float y, float z, float addx, float addy, float addz,
			const std::string& resname, int srcX, int srcY, int srcZ, int w, int h,
			float angle = 0.f, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f,
			float alpha = 255.f, float red = 255.f, float green = 255.f, float blue = 255.f);
	};

	


}



