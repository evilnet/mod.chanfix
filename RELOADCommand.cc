/**
 * RELOADCommand.cc
 *
 * 18/12/2003 - Reed Loden <reed@reedloden.com>
 * Initial Version
 *
 * Unloads the bot's module and reloads it
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 * USA.
 *
 * $Id$
 */

#include	<string>

#include	"gnuworld_config.h"
#include	"StringTokenizer.h"

#include	"chanfix.h"
#include	"responses.h"

RCSTAG("$Id$");

namespace gnuworld
{

void RELOADCommand::Exec(iClient* theClient, sqlUser* theUser, const std::string& Message)
{
StringTokenizer st(Message);
	
if (bot->isUpdateRunning()) {
  bot->SendTo(theClient,
	      bot->getResponse(theUser,
			language::update_in_progress,
			std::string("This command cannot proceed while an update is in progress. Please try again later.")).c_str());
  return;
}

bot->SendTo(theClient,
            bot->getResponse(theUser,
                            language::reloading_client,
                            std::string("Reloading client...see you on the flip side")).c_str());
bot->logAdminMessage("%s (%s) is reloading the chanfix module.",
		     theUser->getUserName().c_str(),
		     theClient->getRealNickUserHost().c_str());

/* Let's save our chanops database. */
bot->prepareUpdate(false);

if (st.size() < 2) {
  server->UnloadClient(bot, "Reloading...");
} else {
  server->UnloadClient(bot, st.assemble(1));
}

server->LoadClient("libchanfix", bot->getConfigFileName());

return;
}

} // namespace gnuworld
