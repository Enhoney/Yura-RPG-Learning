// Copyright Muci Yun.


#include "Player/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindCurrentLevelByExp(int32 CurExp) const
{
	bool bSearching = true;
	int32 CurLevel = 1;
	while (bSearching)
	{
		// 下标对应等级
		if (LevelUpInformation.Num() -1 <= CurLevel)
		{
			return CurLevel;
		}

		/** 注意，这个是升到下一级所需的经验
		 * 如果1到2级需要300经验，第一次走到这里的时候，这个值就是300，
		 * 如果我们由400点经验，就可以升级到2级，
		 * 然后不满足2级到3级的阈值，直接退出，最后得到的就是2级
		 */ 
		if (CurExp >= LevelUpInformation[CurLevel].ExpRequirement)
		{
			CurLevel++;
		}
		else
		{
			bSearching = false;
		}

	}
	return CurLevel;
}
