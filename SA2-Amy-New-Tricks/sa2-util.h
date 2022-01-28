#pragma once

static const void* const PResetAnglePtr = (void*)0x460260;
static inline void PResetAngle(EntityData1* a1, CharObj2Base* co2)
{
	__asm
	{
		mov ebx, [co2]
		mov eax, [a1] // a1
		call PResetAnglePtr
	}
}


//void __usercall PGetFriction(EntityData1* a1@<ecx>, EntityData2* eax0@<eax>, CharObj2Base* a3)
static const void* const PGetFrictionPtr = (void*)0x45F4D0;
static inline void PGetFriction(EntityData1* data, EntityData2* data2, CharObj2Base* co2)
{
	__asm
	{
		push[co2]
		mov eax, [data2]
		mov ecx, [data]
		call PGetFrictionPtr
		add esp, 4

	}
}

FunctionPointer(void, PGetAcceleration, (EntityData1* data1, EntityData2* data2, CharObj2Base* co2), 0x45B610);

//void __usercall PConvertVector_P2G(EntityData1* a1@<edi>, NJS_VECTOR* a2@<esi>)
static const void* const PConvertVP2GPtr = (void*)0x468E70;
static inline void PConvertVector_P2G(EntityData1* a1, NJS_VECTOR* a2)
{
	__asm
	{
		mov esi, [a2]
		mov edi, [a1]
		call PConvertVP2GPtr
	}
}

FunctionPointer(void, Sonic_runsActions, (EntityData1* a1, EntityData2* a2, CharObj2Base* a3, SonicCharObj2* a5), 0x719920);

static const void* const Sonic_CheckNextActionPtr = (void*)0x7220E0;
static inline signed int Sonic_CheckNextAction(SonicCharObj2* a1, EntityData1* a2, EntityData2* a3, CharObj2Base* a4)
{
	signed int result;
	__asm
	{
		push[a4]
		push[a3]
		push[a2]
		mov eax, [a1]
		call Sonic_CheckNextActionPtr
		add esp, 12
		mov result, eax
	}
	return result;
}

FunctionPointer(BOOL, PResetAccelerationAir, (EntityData2* a1, EntityData1* a2, CharObj2Base* a3), 0x46E630);

static const void* const njCalcPointPtr_ = (void*)0x426CC0;
static inline void njCalcPointR(float* matrix, NJS_VECTOR* v, NJS_VECTOR* transform, char additive)
{
	__asm
	{
		movzx eax, [additive]
		push eax
		mov ecx, [transform]
		mov edx, [v]
		mov eax, [matrix]
		call njCalcPointPtr_
		add esp, 4
	}
}

static const void* const njCalcVectorPtr_ = (void*)0x4273B0;
static inline void njCalcVector_(NJS_VECTOR* transform, NJS_VECTOR* v, NJS_MATRIX_PTR m)
{
	__asm
	{
		mov ecx, [m]
		mov edx, [v]
		mov eax, [transform]
		call njCalcVectorPtr_
	}
}

static const void* const njdrawsprite3Dptr = (void*)0x77D690;
static inline void NJDrawSprite3D(int a1, NJS_SPRITE* a2, char a3)
{
	__asm
	{
		push[a3]
		push[a2]
		mov eax, [a1]
		call njdrawsprite3Dptr
		add esp, 8
	}
}

DataPointer(SonicCharObj2*, SonicCO2PtrExtern, 0x1A51A9C);

static const void* const VibeThingPtr = (void*)0x438E70;
static inline void VibeThing(int a1, signed int a2, int a3, signed int a4)
{
	__asm
	{
		push[a4] // int a4
		mov ecx, a3 // a3
		mov edx, a2 // int a2
		mov eax, a1 // a1

		// Call your __cdecl function here:
		call VibeThingPtr
		add esp, 4 // int a4
	}
}

DataPointer(NJS_MATRIX, AmyRightHandMatrix, 0x1A51A3C);
FunctionPointer(void, PlayerAfterImage, (NJS_OBJECT* mdl, NJS_MOTION* mtn, NJS_TEXLIST* texlist, float frame, char pNum), 0x476C20);