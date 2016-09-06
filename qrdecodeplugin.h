/*
    Copyright (C) 2011-2016 Imogen Software Carsten Valdemar Munk

    Contact: Tom Swindell <t.swindell@rubyx.co.uk>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef QRDECODEPLUGIN_H
#define QRDECODEPLUGIN_H

#include <QQmlExtensionPlugin>

class QRDecodePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT

#ifdef QRENCODE_PLUGIN
    Q_PLUGIN_METADATA(IID "stage.rubyx.QRDecode")
#endif

public:
    void registerTypes(const char *uri);
};

#endif // QRDECODEPLUGIN_H
