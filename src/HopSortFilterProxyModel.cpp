/*
 * HopSortFilterProxyModel.cpp is part of Brewtarget, and is Copyright Mik
 * Firestone (mikfire@gmail.com), 2010-2011.
 *
 * Brewtarget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Brewtarget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "brewtarget.h"
#include "HopSortFilterProxyModel.h"
#include "HopTableModel.h"
#include <iostream>

HopSortFilterProxyModel::HopSortFilterProxyModel(QObject *parent) 
: QSortFilterProxyModel(parent)
{
}

bool HopSortFilterProxyModel::lessThan(const QModelIndex &left, 
                                         const QModelIndex &right) const
{
    QVariant leftHop = sourceModel()->data(left);
    QVariant rightHop = sourceModel()->data(right);
    QStringList uses = QStringList() << "Dry Hop" << "Aroma" << "Boil" << "First Wort" << "Mash";
    QModelIndex lSibling, rSibling;
    int lUse, rUse;

   switch( left.column() )
   {
      case HOPALPHACOL:
        return leftHop.toDouble() < rightHop.toDouble();
      case HOPAMOUNTCOL:
      case HOPTIMECOL:
        // Get the indexes of the Use column
        lSibling = left.sibling(left.row(), HOPUSECOL);
        rSibling = right.sibling(right.row(), HOPUSECOL);
        // We are talking to the model, so we get the strings associated with
        // the names, not the Hop::Use enums. We need those translated into
        // ints to make this work
        lUse = uses.indexOf( (sourceModel()->data(lSibling)).toString() );
        rUse = uses.indexOf( (sourceModel()->data(rSibling)).toString() );

        if ( lUse == rUse )
            return Brewtarget::weightQStringToSI(leftHop.toString()) < Brewtarget::weightQStringToSI(rightHop.toString());

        return lUse < rUse;
    }

    return leftHop.toString() < rightHop.toString();
}


