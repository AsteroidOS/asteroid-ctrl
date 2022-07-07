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

    connect(m_batteryService, &BatteryService::ready, this, &Controller::batteryServiceReady);
    connect(m_mediaService, &MediaService::ready, this, &Controller::mediaServiceReady);
    connect(m_notificationService, &NotificationService::ready, this, &Controller::notificationServiceReady);
    connect(m_screenshotService, &ScreenshotService::ready, this, &Controller::screenshotServiceReady);
    connect(m_timeService, &TimeService::ready, this, &Controller::timeServiceReady);
    connect(m_weatherService, &WeatherService::ready, this, &Controller::weatherServiceReady);

    connect(m_mediaService, &MediaService::play, this, &Controller::onPlay);
    connect(m_screenshotService, &ScreenshotService::screenshotReceived, this, &Controller::onScreenshotReceived);

    m_sc = new Scanner(this);
    connect(m_sc, &Scanner::watchFound, this, &Controller::onWatchFound);

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

    m_notificationService->insertNotification("org.asteroid.ctrl", 1, "asteroid-ctrl", "ios-checkmark-circle", "Summary", "Body", "strong");
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

    QList<WeatherDay> weather{
        /* each weather day is icon, lowtemp, hitemp
         * with icons as defined here: https://openweathermap.org/weather-conditions#Weather-Condition-Codes-2
         * and with temperature in K
         * we can specify up to five
         */
        { 200, 273-5, 273+5 },
        { 300, 273-4, 273+4 },
        { 600, 273-3, 273+3 },
        { 701, 273-2, 273+2 },
        { 803, 273-1, 273+1 },
    };
    m_weatherService->setWeatherDays(weather);
}
