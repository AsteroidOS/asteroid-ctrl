/*
 * Copyright (C) 2018 - Florent Revest <revestflo@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    m_watchConnection = new WatchConnection();

    m_batteryService = m_watchConnection->batteryService();
    m_mediaService = m_watchConnection->mediaService();
    m_notificationService = m_watchConnection->notificationService();
    m_screenshotService = m_watchConnection->screenshotService();
    m_timeService = m_watchConnection->timeService();
    m_weatherService = m_watchConnection->weatherService();

    connect(m_batteryService, SIGNAL(ready()), this, SLOT(batteryServiceReady()));
    connect(m_mediaService, SIGNAL(ready()), this, SLOT(mediaServiceReady()));
    connect(m_notificationService, SIGNAL(ready()), this, SLOT(notificationServiceReady()));
    connect(m_screenshotService, SIGNAL(ready()), this, SLOT(screenshotServiceReady()));
    connect(m_timeService, SIGNAL(ready()), this, SLOT(timeServiceReady()));
    connect(m_weatherService, SIGNAL(ready()), this, SLOT(weatherServiceReady()));

    connect(m_mediaService, SIGNAL(play()), this, SLOT(onPlay()));
    connect(m_screenshotService, SIGNAL(screenshotReceived(QByteArray)), this, SLOT(onScreenshotReceived(QByteArray)));

    m_sc = new Scanner(this);
    connect(m_sc, SIGNAL(watchFound(Watch*)), this, SLOT(onWatchFound(Watch*)));

    m_sc->startScan();
}

void Controller::onWatchFound(Watch *w)
{
    m_sc->stopScan();
    m_watchConnection->setDevice(w);
    qInfo() << w->getName() << "found";
}

void Controller::batteryServiceReady()
{
    qInfo() << "Battery service:";
    qInfo() << " level=" << m_batteryService->level();
}

void Controller::mediaServiceReady()
{
    qInfo() << "Media service:";
    qInfo() << " setting dummy music parameters";

    m_mediaService->setTitle("Title");
    m_mediaService->setAlbum("Album");
    m_mediaService->setArtist("Artist");
    m_mediaService->setPlaying(true);
}

void Controller::onPlay()
{
    qInfo() << "Play received";
}

void Controller::notificationServiceReady()
{
    qInfo() << "Notification service:";
    qInfo() << " sending dummy notification";

    m_notificationService->insertNotification("org.asteroid.ctrl", 1, "asteroid-ctrl", "ios-checkmark-circle", "Summary", "Body", NotificationService::Strong);
}

void Controller::screenshotServiceReady()
{
    qInfo() << "Screenshot service:";
    qInfo() << " requesting screenshot";

    m_screenshotService->requestScreenshot();
}

void Controller::onScreenshotReceived(QByteArray data)
{
    qInfo() << " screenshot received";
}

void Controller::timeServiceReady()
{
    qInfo() << "Time service:";
    qInfo() << " syncing time";

    m_timeService->setTime(QDateTime::currentDateTime());
}

void Controller::weatherServiceReady()
{
    qInfo() << "Weather service:";
    qInfo() << " setting dummy weather data";

    m_weatherService->setCity("City");

    QList<short> ids;
    ids.append(200);
    ids.append(300);
    ids.append(600);
    ids.append(701);
    ids.append(800);
    m_weatherService->setIds(ids);

    QList<short> maxTemps;
    ids.append(0);
    ids.append(1);
    ids.append(2);
    ids.append(3);
    ids.append(4);
    m_weatherService->setMaxTemps(maxTemps);

    QList<short> minTemps;
    ids.append(-1);
    ids.append(-2);
    ids.append(-3);
    ids.append(-4);
    ids.append(-5);
    m_weatherService->setMinTemps(minTemps);
}
