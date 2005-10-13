/**
 * BLOCKCommand.cc
 *
 * 07/19/2005 - Reed Loden <reed@reedloden.com>
 * Initial Version
 *
 * Blocks a channel from being fixed, both automatically and manually
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
 * $Id$
 */

#include "gnuworld_config.h"

#include "chanfix.h"
#include "responses.h"
#include "StringTokenizer.h"
#include "sqlChannel.h"
#include "sqlUser.h"

RCSTAG("$Id$");

namespace gnuworld
{

void BLOCKCommand::Exec(iClient* theClient, sqlUser* theUser, const std::string& Message)
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

if (theChan->getFlag(sqlChannel::F_BLOCKED)) {
  bot->SendTo(theClient,
              bot->getResponse(theUser,
                              language::channel_already_blocked,
                              std::string("The channel %s is already blocked.")).c_str(),
                                          theChan->getChannel().c_str());
  return;
}

theChan->setFlag(sqlChannel::F_BLOCKED);
theChan->commit();

/* Add note to the channel about this command */
theChan->addNote(sqlChannel::EV_BLOCK, theUser, st.assemble(2));

bot->SendTo(theClient,
            bot->getResponse(theUser,
                            language::channel_has_been_blocked,
                            std::string("The channel %s has been blocked.")).c_str(),
                                        theChan->getChannel().c_str());

return;
}
} // namespace gnuworld
