/**
 * chanfix_misc.h
 *
 * 17/12/2003 - Matthias Crauwels <ultimate_@wol.be>
 * Initial Version
 *
 * Miscellaneous functions
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

#ifndef __CHANFIX_MISC_H
#define __CHANFIX_MISC_H "$Id$"

namespace gnuworld
{

namespace cf
{

bool compare_points(sqlChanOp* a, sqlChanOp* b) {
  return a->getPoints() > b->getPoints();
}

} //namespace cf

} //namespace gnuworld

#endif // __CHANFIX_MISC_H
