#include "tools/stdafx.h"
int main()
{
	BearCore::BearTimer t;
	while (1)
	{
		Sleep(1);
		printf("%d\r\n", t.get_elapsed_time().asmiliseconds());
		
		t.restart();
	}
	return 0;
}