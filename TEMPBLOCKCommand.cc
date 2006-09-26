/**
 * TEMPBLOCKCommand.cc
 *
 * 19/09/2006 - Neil Spierling
 * Initial Version
 *
 * Temporary blocks a channel from being fixed, both automatically and
 * manually
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 * USA.
 *
 * $Id: BLOCKCommand.cc 1553 2006-04-18 04:03:59Z sirvulcan $
 */

#include "gnuworld_config.h"
#include "Network.h"

#include "chanfix.h"
#include "responses.h"
#include "StringTokenizer.h"
#include "sqlChannel.h"
#include "sqlUser.h"

RCSTAG("$Id: BLOCKCommand.cc 1553 2006-04-18 04:03:59Z sirvulcan $");

namespace gnuworld
{
namespace cf
{

void TEMPBLOCKCommand::Exec(iClient* theClient, sqlUser* theUser, const std::string& Message)
{
StringTokenizer st(Message);

/* Check if channel blocking has been disabled in the config. */
if (!bot->doChanBlocking()) {
  bot->SendTo(theClient,
              bot->getResponse(theUser,
                              language::channel_blocking_disabled,
                              std::string("Channel blocking is disabled.")).c_str());
  return;
}

if (st[1][0] != '#') {
  bot->SendTo(theClient,
              bot->getResponse(theUser,
                              language::invalid_channel_name,
                              std::string("%s is an invalid channel name.")).c_str(),
                                          st[1].c_str());
  return;
}

sqlChannel* theChan = bot->getChannelRecord(st[1]);
if (!theChan) theChan = bot->newChannelRecord(st[1]);

if (!bot->isTempBlocked(theChan->getChannel()))
      bot->tempBlockList.insert(chanfix::tempBlockType::value_type(theChan->getChannel(), bot->currentTime()));
else {
  bot->SendTo(theClient,
              bot->getResponse(theUser,
                              language::channel_already_temp_blocked,
                              std::string("The channel %s is already temp blocked.")).c_str(),
                                          theChan->getChannel().c_str());
  return;
}

/* Add note to the channel about this command */
theChan->addNote(sqlChannel::EV_TEMPBLOCK, theClient, st.assemble(2));

bot->SendTo(theClient,
            bot->getResponse(theUser,
                            language::channel_has_been_temp_blocked,
                            std::string("The channel %s has been temp blocked.")).c_str(),
                                        theChan->getChannel().c_str());

/* Warn and remove from the queue if it is being fixed */
std::string extraLog;
Channel* netChan = Network->findChannel(st[1]);
if (netChan) {
  if (bot->isBeingChanFixed(netChan)) {
    bot->SendTo(theClient,
		bot->getResponse(theUser,
			language::block_aborting_manual_fix,
			std::string("WARNING: Channel %s is being manually fixed; aborting fix as per TEMPBLOCK.")).c_str(),
				    theChan->getChannel().c_str());
    bot->stopFixingChan(netChan, true);
    extraLog = " (current manual fix aborted)";
  }
  if (bot->isBeingAutoFixed(netChan)) {
    bot->SendTo(theClient,
		bot->getResponse(theUser,
			language::block_aborting_auto_fix,
			std::string("WARNING: Channel %s is being automatically fixed; aborting fix as per TEMPBLOCK.")).c_str(),
				    theChan->getChannel().c_str());
    bot->stopFixingChan(netChan, true);
    extraLog = " (current autofix aborted)";
  }
}

/* Log command */
bot->logAdminMessage("%s (%s) TEMPBLOCK %s%s",
		     theUser->getUserName().c_str(),
		     theClient->getRealNickUserHost().c_str(),
		     theChan->getChannel().c_str(),
		     extraLog.c_str());

bot->logLastComMessage(theClient, Message);

return;
}
} // namespace cf
} // namespace gnuworld
