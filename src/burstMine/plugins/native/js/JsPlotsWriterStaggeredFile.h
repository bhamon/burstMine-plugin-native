/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#ifndef CRYO_BURST_MINE_PLUGINS_NATIVE_JS_JS_PLOTS_WRITER_STAGGERED_FILE_H
#define CRYO_BURST_MINE_PLUGINS_NATIVE_JS_JS_PLOTS_WRITER_STAGGERED_FILE_H

#include <memory>
#include <functional>
#include <node.h>
#include <burstMine/js/impl/AsyncData.h>

#include "../PlotsWriterStaggeredFile.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace native {
namespace js {

class JsPlotsWriterStaggeredFile {
	private:
		static v8::Persistent<v8::Function> s_constructor;

	public:
		static void init(v8::Isolate* p_isolate);

		static v8::Handle<v8::Value> New(v8::Isolate* p_isolate, std::unique_ptr<cryo::burstMine::plugins::native::PlotsWriterStaggeredFile>& p_plotsWriter);

	private:
		static void unwrap(const v8::FunctionCallbackInfo<v8::Value>& p_info, std::function<void(cryo::burstMine::plugins::native::PlotsWriterStaggeredFile*)> p_functor);
		static void unwrap(const v8::PropertyCallbackInfo<v8::Value>& p_info, std::function<void(cryo::burstMine::plugins::native::PlotsWriterStaggeredFile*)> p_functor);

		static void getStaggerSize(v8::Local<v8::String> p_property, const v8::PropertyCallbackInfo<v8::Value>& p_info);
		static void getPath(v8::Local<v8::String> p_property, const v8::PropertyCallbackInfo<v8::Value>& p_info);

		static void writePlots(const v8::FunctionCallbackInfo<v8::Value>& p_info);
		static void isClosed(const v8::FunctionCallbackInfo<v8::Value>& p_info);
		static void close(const v8::FunctionCallbackInfo<v8::Value>& p_info);
};

namespace impl {

class AsyncDataWritePlots : public cryo::burstMine::js::impl::AsyncData {
	public:
		cryo::burstMine::ScoopsBuffer* buffer;
		std::size_t size;
		cryo::burstMine::plugins::native::PlotsWriterStaggeredFile* plotsWriter;
};

class AsyncDataClose : public cryo::burstMine::js::impl::AsyncData {
	public:
		v8::Persistent<v8::Object> self;
		cryo::burstMine::plugins::native::PlotsWriterStaggeredFile* plotsWriter;
};

}

}}}}}

#endif
