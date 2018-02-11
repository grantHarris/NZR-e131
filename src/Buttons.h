#include "GPIO.h"
#include "pins.h"

namespace e131Bridge{
	enum class RecPlayback{
		Stopped,
		Recording,
		Playing,
		RecordWait
		RecordingFinished
	}

	enum class Mode{
		Record,
		Playback,
		Live
	}

	class Indicators {
	    public:
	        static Indicators * const indicators;
	    private:
	        Indicators(void) {}
	        std::mutex m_rec_playback_state_mutex;
	        std::mutex m_e131_data_mutex;
	        bool m_e131_data;
	        GPIOClass* m_wifiLedGpio;
			GPIOClass* m_e131DataLedGpio;
			GPIOClass* m_recplayRedLedGpio;
			GPIOClass* m_recplayGreenLedGpio;
			GPIOClass* m_modeRedLedGpio;
			GPIOClass* m_modeGreenLedGpio;
			RecPlayback m_rec_playback_state;
	};

}