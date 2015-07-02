/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#include "PlotsStaggeredFile.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace native {

PlotsStaggeredFile::PlotsStaggeredFile(unsigned long long p_address, unsigned long long p_offset, unsigned int p_number, std::size_t p_staggerSize, const std::string& p_path)
: PlotsStaggered(p_address, p_offset, p_number, p_staggerSize), m_path(p_path) {
}

PlotsStaggeredFile::PlotsStaggeredFile(const PlotsStaggered& p_plots, const std::string& p_path)
: PlotsStaggered(p_plots), m_path(p_path) {
}

PlotsStaggeredFile::~PlotsStaggeredFile() {
}

PlotsStaggeredFile& PlotsStaggeredFile::operator=(const PlotsStaggeredFile& p_other) {
	PlotsStaggered::operator=(p_other);

	m_path = p_other.m_path;

	return *this;
}

}}}}
