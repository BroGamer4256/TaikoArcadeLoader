#include "helpers.h"

namespace patches::JP_NOV_2020 {
void
Init () {
	MessageBoxA (0, "08.18 is currently not supported", 0, 0);
	ExitProcess (0);
}
} // namespace patches::JP_NOV_2020
