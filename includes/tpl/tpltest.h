#pragma once
#include "testmacros.h"

/************	tests	************/
#include "BaseBuffer.h"

/***********************************/



int tpltest() {
#ifdef BaseDynamicBufferTest
	tpl::BaseDynamicBuffer<char>::test();
#endif // BaseDynamicBufferTest

	return 0;
}