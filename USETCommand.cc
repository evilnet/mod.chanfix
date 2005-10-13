/**
 * USETCommand.cc
 *
 * 08/14/2005 - Reed Loden <reed@reedloden.com>
 * Initial Version
 *
 * Sets user <option> to value <value>
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
#include	"sqlUser.h"

RCSTAG("$Id$");

namespace gnuworld
{

void USETCommand::Exec(iClient* theClient, sqlUser* theUser, const std::string& Message)
{
StringTokenizer st(Message);

std::string option = string_upper(st[1]);
std::string value = string_upper(st[2]);

if (option == "NOTICE") {
  if (value == "ON" || value == "YES" || value == "1") {
    theUser->setNotice(true);
    bot->SendTo(theClient,
                bot->getResponse(theUser,
                                language::send_notices,
                                std::string("I will now send you notices.")).c_str());
    theUser->commit();
    return;
  } else if (value == "OFF" || value == "NO" || value == "0") {
    theUser->setNotice(false);
    bot->SendTo(theClient,
                bot->getResponse(theUser,
                                language::send_privmsgs,
                                std::string("I will now send you privmsgs.")).c_str());
    theUser->commit();
    return;
  } else {
    bot->SendTo(theClient,
                bot->getResponse(theUser,
                                language::uset_notice_on_off,
                                std::string("Please use USET NOTICE <on/off>.")).c_str());
    return;
  }
}

if (option == "LANG")
{
  chanfix::languageTableType::iterator ptr = bot->languageTable.find(value);
  if (ptr != bot->languageTable.end()) {
    std::string lang = ptr->second.second;
    theUser->setLanguageId(ptr->second.first);
    theUser->commit();
    bot->SendTo(theClient,
		bot->getResponse(theUser,
			language::lang_set_to,
			std::string("Language is set to %s.")).c_str(), lang.c_str());
    return;
  }

  bot->SendTo(theClient, "ERROR: Invalid language selection.");
  return;
}

bot->SendTo(theClient,
	    bot->getResponse(theUser,
			     language::usetting_doesnt_exist,
			     std::string("This setting does not exist.")).c_str());

return;
}

} // namespace gnuworld
