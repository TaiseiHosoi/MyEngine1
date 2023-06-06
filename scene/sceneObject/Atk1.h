#pragma once
#include "PlayerAction.h"
class Atk1 :
    public PlayerAction
{
protected:
    PlayerActionManager* _pActManager;

public:

    Atk1(PlayerActionManager* pActManager);
    ~Atk1() override;

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
    bool isAtk = false; //�U���s�����̂̃t���O
    bool isNextAtk = false; //���̍U���R���{�ɑJ�ڂ��邩
    bool isCombo1 = false;  //�܂Ђ������U��
    bool isCombo2 = false;  //��]�ːi
    int atkMovePhase = 0;   //���̍s���̒��ōs���̏��Ԃ��Ǘ�����(��:1.�u���X�����߂遨2.�u���X�����)
};
