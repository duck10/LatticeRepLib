#include "FollowControls.h"

// Define static members - these should be removed from Follow.cpp if they're there
std::atomic<bool> FollowControls::g_running{ true };
std::atomic<bool> FollowControls::g_in_input{ false };