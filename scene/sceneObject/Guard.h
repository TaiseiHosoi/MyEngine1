#pragma once
#include "PlayerAction.h"
class Guard :
    public PlayerAction
{
protected:
    PlayerActionManager* _pActManager;

public:

    Guard(PlayerActionManager* pActManager);
    ~Guard() override;

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
   bool isGuard = false;    //�K�[�h
   bool isCounter = false;  //�J�E���^�[�N��
   bool isGuardExcute = false;  //�K�[�h������
   int guardMovePhase = 0;  //�t�F�[�Y�Ǘ�
   int counterFrameCount = 0;   //�t�F�C�Y�Ǘ��p�J�E���g
   Vector3 guardKnockVel = {};  //�m�b�N�o�b�N����vel
   std::vector<SphereCollider*>* sphere_;   //�����蔻��|�C���^

};