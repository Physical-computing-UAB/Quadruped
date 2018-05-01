#include "CMachineState.h"

// === Constructor ===
CMachineState::CMachineState()
{
  m_isActive = true;

  for (size_t i = 0; i < MAX_MATRIX_SIZE; i++)
  {
    for (size_t j = 0; j < MAX_MATRIX_SIZE; j++) {
      m_map[i][j] = 1;
    }
  }
}
