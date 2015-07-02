/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#include <uv.h>

#include "JsPluginNative.h"
#include "JsPlotsStaggeredFile.h"
#include "JsPlotsWriterStaggeredFile.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace native {
namespace js {

void JsPluginNative::init(v8::Handle<v8::Object> p_exports) {
	JsPlotsWriterStaggeredFile::init(v8::Isolate::GetCurrent());

	p_exports->Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "createPlotsWriter"), v8::FunctionTemplate::New(v8::Isolate::GetCurrent(), createPlotsWriter)->GetFunction());
}

void JsPluginNative::createPlotsWriter(const v8::FunctionCallbackInfo<v8::Value>& p_info) {
	v8::HandleScope scope(p_info.GetIsolate());

	if(p_info.Length() < 1) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Missing [plots] parameter")));
		return;
	} else if(p_info.Length() < 2) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Missing [callback] parameter")));
		return;
	} else if(!p_info[0]->IsObject()) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Invalid [plots] parameter")));
		return;
	} else if(!p_info[1]->IsFunction()) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Invalid [callback] parameter")));
		return;
	}

	std::unique_ptr<cryo::burstMine::plugins::native::js::impl::AsyncDataCreateWriter> asyncData(new cryo::burstMine::plugins::native::js::impl::AsyncDataCreateWriter);
	asyncData->isolate = p_info.GetIsolate();
	asyncData->callback.Reset(p_info.GetIsolate(), v8::Local<v8::Function>::Cast(p_info[1]));

	try {
		asyncData->plots = std::unique_ptr<cryo::burstMine::plugins::native::PlotsStaggeredFile>(
			new cryo::burstMine::plugins::native::PlotsStaggeredFile(
				JsPlotsStaggeredFile::FromObject(p_info.GetIsolate(), v8::Local<v8::Object>::Cast(p_info[0]))
			)
		);
	} catch(const std::exception& p_ex) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), p_ex.what())));
		return;
	}

	uv_work_t* work = new uv_work_t;
	work->data = static_cast<void*>(asyncData.release());

	uv_queue_work(
		uv_default_loop(),
		work,
		[](uv_work_t* p_work) {
			cryo::burstMine::plugins::native::js::impl::AsyncDataCreateWriter* asyncData = static_cast<cryo::burstMine::plugins::native::js::impl::AsyncDataCreateWriter*>(p_work->data);

			try {
				asyncData->plotsWriter = std::unique_ptr<cryo::burstMine::plugins::native::PlotsWriterStaggeredFile>(
					new cryo::burstMine::plugins::native::PlotsWriterStaggeredFile(*asyncData->plots)
				);
			} catch(const std::exception&) {
				asyncData->exception = std::current_exception();
			}
		},
		[](uv_work_t* p_work, int p_status) {
			cryo::burstMine::plugins::native::js::impl::AsyncDataCreateWriter* asyncData = static_cast<cryo::burstMine::plugins::native::js::impl::AsyncDataCreateWriter*>(p_work->data);

			v8::Handle<v8::Value> jsPlotsWriter = JsPlotsWriterStaggeredFile::New(asyncData->isolate, asyncData->plotsWriter);

			asyncData->triggerCallback({jsPlotsWriter});
			delete asyncData;
			delete p_work;
		}
	);
}

}}}}}

NODE_MODULE(native, cryo::burstMine::plugins::native::js::JsPluginNative::init);
