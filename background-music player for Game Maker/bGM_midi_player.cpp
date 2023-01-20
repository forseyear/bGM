#include "bGM_midi_player.hpp"

CBgmidiPlayer::CBgmidiPlayer()
{
}

CBgmidiPlayer::~CBgmidiPlayer()
{
}

void CBgmidiPlayer::init(sound_options_p const &so)
{
    CCharacterConverter cc;
    m_sound_options_ = so;
    m_pBgmConsole_.reset(new CBgmConsole(m_sound_options_.isDebuging));

    if (m_sound_options_.isDebuging)
    {
        m_pBgmConsole_->Create();
    }
    m_pBgmConsole_->Init();

    m_midi_device_ = audiere::OpenMIDIDevice("MCI");
    if (m_midi_device_ != 0)
    {
        m_pBgmConsole_->DoneOpenDevice();
    } else {
        m_pBgmConsole_->FailedOpenDevice();
    }

    m_midi_stream_ = m_midi_device_->openStream(cc.ToString(m_sound_options_.filename).data());

    if (m_midi_stream_ != 0)
    {
        m_pBgmConsole_->DoneLoadStream();
    } else {
        tstring filename = cc.ToTstring(m_sound_options_.filename.data());
        m_pBgmConsole_->FailedLoadStream(filename);
    }

    m_midi_stream_->setRepeat(m_sound_options_.isLooping);

    if (!m_sound_options_.isLoadOnly)
    {
        firstPlay();
    }

    std::string devicename = m_midi_device_->getName();
    if (m_midi_stream_ != 0)
    {
        m_pBgmConsole_->DoneStartPlayback(cc.ToTstring(devicename.data()), m_sound_options_.filename);
        printSoundOptions();
    } else {
        m_pBgmConsole_->FailedStartPlayback();
    }

    m_sound_start_ = 0;
    m_sound_end_ = m_midi_stream_->getLength();
}

void CBgmidiPlayer::firstPlay()
{
    m_sound_options_.isPlaying = true;
    m_midi_stream_->play();
}

void CBgmidiPlayer::play()
{
    m_sound_options_.isPlaying = true;
    m_midi_stream_->play();
    printSoundOptions();
}

void CBgmidiPlayer::pause()
{
    if (m_sound_options_.isPlaying)
    {
        m_sound_options_.isPlaying = false;
        m_midi_stream_->stop();
    } else {
        m_sound_options_.isPlaying = true;
        m_midi_stream_->play();
    }
    printSoundOptions();
}

void CBgmidiPlayer::reset()
{
    m_midi_stream_->setPosition(0);
    printSoundOptions();
}

void CBgmidiPlayer::stop()
{
    m_sound_options_.isPlaying = false;
    m_midi_stream_->stop();
    m_midi_stream_->setPosition(0);
    printSoundOptions();
}

void CBgmidiPlayer::setLoop(bool isLooping)
{
    m_sound_options_.isLooping = isLooping;
    m_midi_stream_->setRepeat(m_sound_options_.isLooping);
    printSoundOptions();
}

void CBgmidiPlayer::setPosition(int position)
{
    m_midi_stream_->setPosition(soundRange(position));
}

bool CBgmidiPlayer::getisLooping() const
{
    return m_sound_options_.isLooping;
}

void CBgmidiPlayer::writePosition()
{
    tempcreater.Create((tstring)_T("GetPositionTemp"), m_midi_stream_->getPosition());
}

void CBgmidiPlayer::writeLength()
{
    tempcreater.Create((tstring)_T("GetLengthTemp"), m_midi_stream_->getLength());
}

void CBgmidiPlayer::printSoundOptions()
{
    m_pBgmConsole_->UpdateMIDIStatus(
        m_sound_options_.isLooping,
        !m_sound_options_.isPlaying
    );
}

int CBgmidiPlayer::soundRange(const int &number)
{
    return maxmin(m_sound_end_, m_sound_start_, number);
}
