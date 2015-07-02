/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#include <sstream>
#include <burstMine/constants.h>

#include "PlotsWriterStaggeredFile.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace native {

PlotsWriterStaggeredFile::PlotsWriterStaggeredFile(const PlotsStaggeredFile& p_plots) throw (std::exception)
: cryo::burstMine::PlotsWriterStaggered(p_plots.getStaggerSize()) {
	std::ostringstream absolutePath;
	absolutePath << p_plots.getPath() << "/" << p_plots.getAddress() << "_" << p_plots.getOffset() << "_" << p_plots.getNumber() << "_" << p_plots.getStaggerSize();

	m_path = absolutePath.str();
	m_file = std::ofstream(m_path, std::ofstream::out | std::ofstream::app);
}

PlotsWriterStaggeredFile::~PlotsWriterStaggeredFile() {
}

void PlotsWriterStaggeredFile::flushBuffer() {
	void* data = static_cast<void*>(&m_buffer[0]);
	m_file.write(static_cast<char*>(data), m_staggerSize * cryo::burstMine::PLOT_SIZE);
}

}}}}
