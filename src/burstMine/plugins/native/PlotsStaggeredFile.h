/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#ifndef CRYO_BURST_MINE_PLUGINS_NATIVE_PLOTS_STAGGERED_FILE_H
#define CRYO_BURST_MINE_PLUGINS_NATIVE_PLOTS_STAGGERED_FILE_H

#include <string>
#include <burstMine/PlotsStaggered.h>

namespace cryo {
namespace burstMine {
namespace plugins {
namespace native {

class PlotsStaggeredFile : public cryo::burstMine::PlotsStaggered {
	private:
		std::string m_path;

	public:
		PlotsStaggeredFile(unsigned long long p_address, unsigned long long p_offset, unsigned int p_number, std::size_t p_staggerSize, const std::string& p_path);
		PlotsStaggeredFile(const PlotsStaggered& p_plots, const std::string& p_path);
		virtual ~PlotsStaggeredFile();

		PlotsStaggeredFile& operator=(const PlotsStaggeredFile& p_other);

		inline std::string getPath() const;
		inline void setPath(const std::string& p_path);
};

}}}}

namespace cryo {
namespace burstMine {
namespace plugins {
namespace native {

std::string PlotsStaggeredFile::getPath() const {
	return m_path;
}

void PlotsStaggeredFile::setPath(const std::string& p_path) {
	m_path = p_path;
}

}}}}

#endif
