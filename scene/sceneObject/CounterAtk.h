#pragma once
#include "PlayerAction.h"
class CounterAtk :
    public PlayerAction
{
protected:
    PlayerActionManager* _pActManager;

public:

    CounterAtk(PlayerActionManager* pActManager);
    ~CounterAtk() override;

    void Initialize(FBXObject3d* gameObject) override;
    void Update(Input* input) override;
    void Draw() override;

private:

    Input* input_ = Input::GetInstance();
private:   //�ǂ̍s���ł��K�v
    int animNum;    //���ԃA�j���[�V������
    int animFlameCT;    //�t���[�����J�E���g(�A�j���[�V�����̃C���^�[�t�F�[�X�p)
    FBXObject3d* gameObject_;

private:    //�ŗL�ŕK�v�Ȃ���
    //�t�F�[�Y�̃^�[�j���O�|�C���g
    const int max2animNum = 30;	//�T�}�[�\���g�̍ő�t���[��
    const int max3animNum = max2animNum + 25;	//��]���ė͂����߂�ő�t���[��
    const int max4animNum = max3animNum + 20;
    const int max5animNum = max4animNum + 45;
    const int max6animNum = max5animNum + 35;
    
    int counterFrameCount = 0;  //�t�F�[�Y�Ǘ��p�J�E���^
    int phase = 0;  //�t�F�[�Y
    float kAccumulateRotVel = 0;    //��]�̑��x�������x�I�ɒ~�ς����邽�߂̕ϐ�
};