/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Tests/client/SharedUtil_impl.cpp
 *  PURPOSE:     Provide SharedUtil implementations for the test project
 *               without pulling in heavy vendor dependencies (CryptoPP, bcrypt).
 *
 *  This mirrors Shared/sdk/SharedUtil.cpp but deliberately skips
 *  SharedUtil.Hash.hpp (which requires bcrypt + SHA vendor sources).
 *  Tests that exercise crypto/hash functionality should be added once
 *  those vendors are linked into the test project.
 *
 *****************************************************************************/

#include <SharedUtil.h>

// Core string/formatting implementations
#include "../../Shared/sdk/SString.hpp"
#include "../../Shared/sdk/WString.hpp"

// Allocation tracking (mostly macros, lightweight)
#include "../../Shared/sdk/SharedUtil.AllocTracking.hpp"

// Pure utility implementations used by our tests
#include "../../Shared/sdk/SharedUtil.Misc.hpp"
#include "../../Shared/sdk/SharedUtil.File.hpp"
#include "../../Shared/sdk/SharedUtil.Time.hpp"
#include "../../Shared/sdk/SharedUtil.Game.hpp"

// Intentionally skipped: SharedUtil.Hash.hpp (needs bcrypt vendor)

#include "../../Shared/sdk/SharedUtil.Profiling.hpp"
#include "../../Shared/sdk/SharedUtil.Logging.hpp"
#include "../../Shared/sdk/SharedUtil.AsyncTaskScheduler.hpp"
#include "../../Shared/sdk/SharedUtil.Memory.hpp"
#include "../../Shared/sdk/UTF8.hpp"
