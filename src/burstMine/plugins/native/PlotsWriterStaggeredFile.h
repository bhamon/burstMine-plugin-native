/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#ifndef CRYO_BURST_MINE_PLUGINS_NATIVE_PLOTS_WRITER_STAGGERED_FILE_H
#define CRYO_BURST_MINE_PLUGINS_NATIVE_PLOTS_WRITER_STAGGERED_FILE_H

#include <string>
#include <fstream>
#include <exception>
#include <burstMine/PlotsWriterStaggered.h>

#include "PlotsStaggeredFile.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace native {

class PlotsWriterStaggeredFile : public cryo::burstMine::PlotsWriterStaggered {
	private:
		std::string m_path;
		std::ofstream m_file;

	public:
		PlotsWriterStaggeredFile(const PlotsStaggeredFile& p_plots) throw (std::exception);
		virtual ~PlotsWriterStaggeredFile();

		PlotsWriterStaggeredFile& operator=(const PlotsWriterStaggeredFile& p_other) = delete;

		inline std::string getPath() const;

	protected:
		virtual void flushBuffer();
};

}}}}

namespace cryo {
namespace burstMine {
namespace plugins {
namespace native {

std::string PlotsWriterStaggeredFile::getPath() const {
	return m_path;
}

}}}}

#endif
