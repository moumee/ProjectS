


#include "ActorComponents/WeaponSystem/WeaponInterface.h"
#include "ActorComponents/WeaponSystem/ArmRecoilStruct.h"

// Add default functionality here for any IWeaponInterface functions that are not pure virtual.

void IWeaponInterface::SetRightHandToAimSocketOffset(FVector offset)
{
}

void IWeaponInterface::ZoomIn(bool bZoomIn)
{
}

void IWeaponInterface::ReloadingEnd()
{
}

void IWeaponInterface::SwitchToOtherWeapon()
{
}

void IWeaponInterface::AddArmRecoil(FArmRecoilStruct* armrecoil, float AdditionalRecoilAmountX, float AdditionalRecoilAmountY, float AdditionalRecoilAmountZ)
{
}
