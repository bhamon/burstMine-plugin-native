/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#ifndef CRYO_BURST_MINE_JS_PLUGINS_NATIVE_JS_PLOTS_STAGGERED_FILE_H
#define CRYO_BURST_MINE_JS_PLUGINS_NATIVE_JS_PLOTS_STAGGERED_FILE_H

#include <exception>
#include <node.h>

#include "../PlotsStaggeredFile.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace native {
namespace js {

class JsPlotsStaggeredFile {
	public:
		static v8::Handle<v8::Value> New(v8::Isolate* p_isolate, const cryo::burstMine::plugins::native::PlotsStaggeredFile& p_plots);
		static cryo::burstMine::plugins::native::PlotsStaggeredFile FromObject(v8::Isolate* p_isolate, const v8::Handle<v8::Object>& p_object) throw (std::exception);
};

}}}}}

#endif
