#include "bGM_player.hpp"

CBgmPlayer::CBgmPlayer()
{
}

CBgmPlayer::~CBgmPlayer()
{
}

void CBgmPlayer::init(sound_options_p const &so)
{
    CCharacterConverter cc;
    m_sound_options_ = so;
    m_pBgmConsole_.reset(new CBgmConsole(m_sound_options_.isDebuging));

    if (m_sound_options_.isDebuging)
    {
        m_pBgmConsole_->Create();
    }
    m_pBgmConsole_->Init();

    m_audio_device_ = audiere::OpenDevice();
    if (m_audio_device_ != 0) {
        m_pBgmConsole_->DoneOpenDevice();
    } else {
        m_pBgmConsole_->FailedOpenDevice();
    }

    m_loop_point_ = audiere::CreateLoopPointSource(audiere::OpenSampleSource(cc.ToString(m_sound_options_.filename).data()));
    m_sample_source_ = m_loop_point_.get();
    m_output_stream_ = audiere::OpenSound(m_audio_device_, m_sample_source_, true);

    if (m_sample_source_ != 0 && m_loop_point_ != 0 && m_output_stream_ != 0)
    {
        m_pBgmConsole_->DoneLoadStream();
    } else {
        tstring filename = cc.ToTstring(m_sound_options_.filename.data());
        m_pBgmConsole_->FailedLoadStream(filename);
    }

    m_output_stream_->setPan(m_sound_options_.pan);
    m_output_stream_->setPitchShift(m_sound_options_.pitch);
    m_output_stream_->setRepeat(m_sound_options_.isLooping);
    m_output_stream_->setVolume(m_sound_options_.volume);

    if (!m_sound_options_.isLoadOnly)
    {
        firstPlay();
    }

    std::string devicename = m_audio_device_->getName();
    if (m_output_stream_ != 0)
    {
        m_pBgmConsole_->DoneStartPlayback(cc.ToTstring(devicename.data()), m_sound_options_.filename);
        printSoundOptions();
    } else {
        m_pBgmConsole_->FailedStartPlayback();
    }

    m_sound_start_ = 0;
    m_sound_end_ = m_output_stream_->getLength();
}

void CBgmPlayer::fadeIn(int fadeFrame, float volume)
{
    float fadedVolume = volumeRange(volume);
    std::chrono::milliseconds interval((int)fadeFrame / m_fade_count_);
    std::thread thFade([&] { executeFadeIn(interval, fadedVolume); });
    thFade.detach();
}

void CBgmPlayer::fadeOut(int fadeFrame, float volume)
{
    float fadedVolume = volumeRange(volume);
    std::chrono::milliseconds interval((int)fadeFrame / m_fade_count_);
    std::thread thFade([&] { executeFadeOut(interval, fadedVolume); });
    thFade.detach();
}

void CBgmPlayer::executeFadeIn(std::chrono::milliseconds interval, float fadedVolume)
{
    m_need_StopFade_ = false;
    float beforeVolume = m_output_stream_->getVolume();
    float volumeAcceleration = (fadedVolume - m_output_stream_->getVolume()) / m_fade_count_;
    int loopCount = 1;

    while (loopCount < m_fade_count_)
    {
        if (m_need_StopFade_) {
            break;
        }

        if (m_sound_options_.isPlaying)
        {
            float nowVolume = (volumeAcceleration * loopCount) + beforeVolume;
            m_output_stream_->setVolume(nowVolume);
            loopCount++;

            m_sound_options_.volume = nowVolume;
            printSoundOptions();
        }
        std::this_thread::sleep_for(interval * 1000);
    }
    setVolume(fadedVolume);
    printSoundOptions();
}

void CBgmPlayer::executeFadeOut(std::chrono::milliseconds interval, float fadedVolume)
{
    m_need_StopFade_ = false;
    float beforeVolume = m_output_stream_->getVolume();
    float volumeAcceleration = (m_output_stream_->getVolume() - fadedVolume) / m_fade_count_;
    int loopCount = 1;

    while (loopCount < m_fade_count_)
    {
        if (m_need_StopFade_) {
            break;
        }

        if (m_sound_options_.isPlaying)
        {
            float nowVolume = beforeVolume - (volumeAcceleration * loopCount);
            m_output_stream_->setVolume(nowVolume);
            loopCount++;

            m_sound_options_.volume = nowVolume;
            printSoundOptions();
        }
        std::this_thread::sleep_for(interval * 1000);
    }
    setVolume(fadedVolume);
    printSoundOptions();
}

void CBgmPlayer::firstPlay()
{
    m_sound_options_.isPlaying = true;
    m_output_stream_->play();
}

void CBgmPlayer::play()
{
    m_sound_options_.isPlaying = true;
    m_output_stream_->play();
    printSoundOptions();
}

void CBgmPlayer::pause()
{
    if (m_sound_options_.isPlaying)
    {
        m_sound_options_.isPlaying = false;
        m_output_stream_->stop();
    } else {
        m_sound_options_.isPlaying = true;
        m_output_stream_->play();
    }
    printSoundOptions();
}

void CBgmPlayer::reset()
{
    stopFade();
    m_output_stream_->reset();
    printSoundOptions();
}

void CBgmPlayer::stop()
{
    stopFade();
    m_sound_options_.isPlaying = false;
    m_output_stream_->stop();
    m_output_stream_->setPosition(0);
    printSoundOptions();
}

void CBgmPlayer::setVolume(float volume)
{
    stopFade();
    m_sound_options_.volume = volumeRange(volume);
    m_output_stream_->setVolume(m_sound_options_.volume);
    printSoundOptions();
}

void CBgmPlayer::setPan(float pan)
{
    stopFade();
    m_sound_options_.pan = panRange(pan);
    m_output_stream_->setPan(m_sound_options_.pan);
    printSoundOptions();
}

void CBgmPlayer::setPitch(float pitch)
{
    stopFade();
    m_sound_options_.pitch = pitchRange(pitch);
    m_output_stream_->setPitchShift(m_sound_options_.pitch);
    printSoundOptions();
}

void CBgmPlayer::addRepeat(int repeatPositionA, int repeatPositionB)
{
    if (!m_sound_options_.isLooping)
    {
        setLoop(true);
    }
    m_repeat_positionA_ = soundRange(repeatPositionA);
    m_repeat_positionB_ = soundRange(repeatPositionB);

    m_loop_point_->addLoopPoint(m_repeat_positionA_, m_repeat_positionB_, 0);
}

void CBgmPlayer::setRepeatCount(int repeatCount)
{
    if (!m_sound_options_.isLooping)
    {
        setLoop(true);
    }
    removeRepeat();
    m_loop_point_->addLoopPoint(m_repeat_positionA_, m_repeat_positionB_, repeatCount);
}

void CBgmPlayer::removeRepeat()
{
    m_loop_point_->removeLoopPoint(0);
}

void CBgmPlayer::checkRepeat()
{
    m_output_stream_->setPosition(((m_repeat_positionA_ - 200000) > 0) ? (m_repeat_positionA_ - 200000) : m_repeat_positionA_);
}

void CBgmPlayer::setLoop(bool isLooping)
{
    m_sound_options_.isLooping = isLooping;
    m_output_stream_->setRepeat(m_sound_options_.isLooping);
    printSoundOptions();
}

void CBgmPlayer::setPosition(int position)
{
    m_output_stream_->setPosition(soundRange(position));
}

bool CBgmPlayer::getisLooping() const
{
    return m_sound_options_.isLooping;
}

void CBgmPlayer::writePosition()
{
    tempcreater.Create((tstring)_T("GetPositionTemp"), m_output_stream_->getPosition());
}

void CBgmPlayer::writeLength()
{
    tempcreater.Create((tstring)_T("GetLengthTemp"), m_output_stream_->getLength());
}

void CBgmPlayer::printSoundOptions()
{
    m_pBgmConsole_->UpdateStatus(
        m_sound_options_.isLooping,
        !m_sound_options_.isPlaying,
        m_sound_options_.volume,
        m_sound_options_.pan,
        m_sound_options_.pitch
    );
}

int CBgmPlayer::soundRange(const int &number)
{
    return maxmin(m_sound_end_, m_sound_start_, number);
}

float CBgmPlayer::volumeRange(const float &volume)
{
    return maxmin(0.0f, 1.0f, volume);
}

float CBgmPlayer::panRange(const float &pan)
{
    return maxmin(1.0f, -1.0f, pan);
}

float CBgmPlayer::pitchRange(const float &pitch)
{
    return maxmin(0.5f, 2.0f, pitch);
}

template <typename T>
T CBgmPlayer::maxmin(const T &max, const T &min, const T &number)
{
    T small = std::min(max, number);
    return std::max(min, small);
}
