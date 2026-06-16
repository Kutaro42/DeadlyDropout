/**
 * @file musiccontroller.cpp
 * @brief Implémentation du MusicController singleton.
 */
#include "musiccontroller.h"

MusicController::MusicController(QObject* parent)
    : QObject(parent)
    , player(new QMediaPlayer(this))
    , audioOutput(new QAudioOutput(this))
    , lastIndex(-1)
{
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.2f);

    // ── Liste des musiques ingame
    QString base = QCoreApplication::applicationDirPath() + "/music/";
    gameTracks << base + "Zeal & Ardor - Götterdämmerung.ogg"
               << base + "Wormhole Inversion.ogg"
               << base + "Viande.ogg"
               << base + "To Ost-Paris.ogg"
               << base + "TNO OST Lebensraum.ogg"
               << base + "The Toxic Avenger - My Only Chance.ogg"
               << base + "The Atomic Siege - Ayden George.ogg"
               << base + "Molchat Doma - Судно (Борис Рижий).ogg"
               << base + "Mittsies - Vitality.ogg"
               << base + "Mick Gordon - 02. Rip & Tear.ogg"
               << base + "Locknar - Crawler.ogg"
               << base + "Ho99o9 (Horror) - DeathKult Disciples (999 Anthem).ogg"
               << base + "Fanning the Flames - Ayden George.ogg"
               << base + "Eyes of Abomination - Malodorous.ogg"
               << base + "BYEBYE.WAV - SXCREDMANE.ogg"
               << base + "Bring Me The Horizon - Can You Feel My Heart - Metalfanaus.ogg"
               << base + "assadist pussy got me actin strange.ogg";
}

void MusicController::playTrack(const QString& resourcePath)
{
    player->setSource(QUrl::fromLocalFile(resourcePath));
    player->setLoops(QMediaPlayer::Infinite);
    player->play();
}

void MusicController::playMenu()
{
    QString path = QCoreApplication::applicationDirPath() + "/music/menu.ogg";
    player->setSource(QUrl::fromLocalFile(path));
    player->setLoops(QMediaPlayer::Infinite);
    player->play();
}

void MusicController::stop()
{
    player->stop();
}

void MusicController::playRandomGame()
{
    if (gameTracks.isEmpty()) return;

    int index;
    if (gameTracks.size() == 1) {
        index = 0;
    } else {
        do {
            index = static_cast<int>(
                QRandomGenerator::global()->bounded(gameTracks.size()));
        } while (index == lastIndex);
    }
    lastIndex = index;
    playTrack(gameTracks[index]);
}

void MusicController::togglePause()
{
    if (player->playbackState() == QMediaPlayer::PlayingState)
        player->pause();
    else
        player->play();
}

void MusicController::nextTrack()
{
    playRandomGame();
}

bool MusicController::isPlaying() const
{
    return player->playbackState() == QMediaPlayer::PlayingState;
}
