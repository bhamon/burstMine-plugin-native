/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#include <stdexcept>
#include <burstMine/js/JsPlotsStaggered.h>

#include "JsPlotsStaggeredFile.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace native {
namespace js {

v8::Handle<v8::Value> JsPlotsStaggeredFile::New(v8::Isolate* p_isolate, const cryo::burstMine::plugins::native::PlotsStaggeredFile& p_plots) {
	v8::EscapableHandleScope scope(p_isolate);

	v8::Local<v8::Object> obj = cryo::burstMine::js::JsPlotsStaggered::New(p_isolate, p_plots)->ToObject();

	v8::Local<v8::Object> params = obj->Get(v8::String::NewFromUtf8(p_isolate, "params"))->ToObject();
	params->Set(v8::String::NewFromUtf8(p_isolate, "path"), v8::String::NewFromUtf8(p_isolate, p_plots.getPath().c_str()));

	return scope.Escape(obj);
}

cryo::burstMine::plugins::native::PlotsStaggeredFile JsPlotsStaggeredFile::FromObject(v8::Isolate* p_isolate, const v8::Handle<v8::Object>& p_object) throw (std::exception) {
	v8::HandleScope scope(p_isolate);

	cryo::burstMine::PlotsStaggered plots(cryo::burstMine::js::JsPlotsStaggered::FromObject(p_isolate, p_object));

	v8::Local<v8::Object> params = p_object->Get(v8::String::NewFromUtf8(p_isolate, "params"))->ToObject();

	if(!params->Has(v8::String::NewFromUtf8(p_isolate, "path"))) {
		throw std::runtime_error("Missing [params.path] parameter");
	} else if(!params->Get(v8::String::NewFromUtf8(p_isolate, "path"))->IsString()) {
		throw std::runtime_error("Invalid [params.path] parameter");
	}

	v8::String::Utf8Value path(params->Get(v8::String::NewFromUtf8(p_isolate, "path")));
	return cryo::burstMine::plugins::native::PlotsStaggeredFile(plots, *path);
}

}}}}}
