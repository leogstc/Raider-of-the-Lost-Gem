#pragma once

namespace gameover{
	//�Q�[���I�[�o�[���ɐ؂�ւ�鎞�Ԃ��w�肷��B
	//�f�t�H���g��3�b
	//�C�ӕύX�̏ꍇ��Start�ōs������
	void SetTime(float time);
	//�I�������J�n
	//�Q�[���I�[�o�[�������ɌĂяo��
	void Start();
	//�I������������
  void Init(float time);
	//�I����������
	//�L�����N�^�̈ړ��}�~�Ȃǂ�
	//StartGameOver�Ăяo�����true
	//InitGameOver��false+
	bool isGameOver();
	//�I�����X�e�[�W�ڍs����
	//StartGameOver��Ɉ�莞�Ԃŉ�ʐ؂�ւ������s����B
	//next : �؂�ւ���X�e�[�W�̔ԍ�
	void Step(int next);
}

