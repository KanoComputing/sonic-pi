/*
 * dbus_actions.cpp
 *
 * Copyright (C) 2015 Kano Computing Ltd.
 * License: http://www.gnu.org/licenses/gpl-2.0.txt GNU General Public License v2
 *
 * me.kano.sonicpi.Actions methods
 *
 */

#include "dbus_actions.h"

/*
 * Implementation of adaptor class DbusActionsAdaptor
 */

DbusActionsAdaptor::DbusActionsAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

DbusActionsAdaptor::~DbusActionsAdaptor()
{
    // destructor
}

void DbusActionsAdaptor::load()
{
    // handle method call me.kano.sonicpi.load
    QMetaObject::invokeMethod(parent(), "load");
}

void DbusActionsAdaptor::launch_share(const QString share_filename)
{
    // handle method call me.kano.sonicpi.load_share, passing the filename to load into workspace.
    QMetaObject::invokeMethod(parent(), "load_share", Qt::AutoConnection, Q_ARG(QString, share_filename));
}

void DbusActionsAdaptor::make()
{
    // handle method call me.kano.sonicpi.make
    QMetaObject::invokeMethod(parent(), "runCode");
}

void DbusActionsAdaptor::save()
{
    // handle method call me.kano.sonicpi.save
    QMetaObject::invokeMethod(parent(), "saveDialog");
}

void DbusActionsAdaptor::share()
{
    // handle method call me.kano.sonicpi.share
    QMetaObject::invokeMethod(parent(), "shareDialog");
}

