#include "Framework.h"


void GetCardResult::Clear()
{
	cards.clear();
	cards.shrink_to_fit();
}


void GetSupplierResult::Clear()
{
	supplier = nullptr;
}
