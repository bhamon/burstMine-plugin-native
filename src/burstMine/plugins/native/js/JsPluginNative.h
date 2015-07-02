/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#ifndef CRYO_BURST_MINE_PLUGINS_NATIVE_JS_JS_PLUGIN_NATIVE_H
#define CRYO_BURST_MINE_PLUGINS_NATIVE_JS_JS_PLUGIN_NATIVE_H

#include <memory>
#include <node.h>
#include <burstMine/js/impl/AsyncData.h>

#include "../PlotsStaggeredFile.h"
#include "../PlotsWriterStaggeredFile.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace native {
namespace js {

class JsPluginNative {
	public:
		static void init(v8::Handle<v8::Object> p_exports);

	private:
		static void createPlotsWriter(const v8::FunctionCallbackInfo<v8::Value>& p_info);
};

namespace impl {

class AsyncDataCreateWriter : public cryo::burstMine::js::impl::AsyncData {
	public:
		std::unique_ptr<cryo::burstMine::plugins::native::PlotsStaggeredFile> plots;
		std::unique_ptr<cryo::burstMine::plugins::native::PlotsWriterStaggeredFile> plotsWriter;
};

}

}}}}}

#endif
