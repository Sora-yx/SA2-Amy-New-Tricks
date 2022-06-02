#pragma once


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


DataPointer(NJS_MATRIX, AmyRightHandMatrix, 0x1A51A3C);
DataPointer(float, flt_1283704, 0x1283704);

static const void* const DoHomingAttackEffectPtr = (void*)0x720E00;
static inline void DoHomingAttackEffect(SonicCharObj2* sCo2, EntityData1* a2, EntityData2* a3, CharObj2Base* co2)
{
	__asm
	{
		push[co2]
		push[a3]
		push[a2]
		mov ebx, sCo2
		call DoHomingAttackEffectPtr
		add esp, 12
	}
}

static const void* const LightDashEffectPtr = (void*)0x723FC0;
static inline int LightDashAllowed(EntityData1* a1, CharObj2Base* a2)
{
	int result;
	__asm
	{
		mov esi, [a2]
		mov eax, [a1]
		call LightDashEffectPtr
		mov result, eax
	}

	return result;
}
