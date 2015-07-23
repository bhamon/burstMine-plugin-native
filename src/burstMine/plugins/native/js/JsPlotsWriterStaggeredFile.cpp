/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#include <uv.h>
#include <burstMine/js/JsScoopsBuffer.h>

#include "JsPlotsWriterStaggeredFile.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace native {
namespace js {

v8::Persistent<v8::Function> JsPlotsWriterStaggeredFile::s_constructor;

void JsPlotsWriterStaggeredFile::init(v8::Isolate* p_isolate) {
	v8::HandleScope scope(p_isolate);

	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(p_isolate);
	tpl->SetClassName(v8::String::NewFromUtf8(p_isolate, "PlotsWriterStaggeredFile"));

	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	tpl->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(p_isolate, "staggerSize"), JsPlotsWriterStaggeredFile::getStaggerSize);
	tpl->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(p_isolate, "path"), JsPlotsWriterStaggeredFile::getPath);

	tpl->PrototypeTemplate()->Set(v8::String::NewFromUtf8(p_isolate, "writePlots"), v8::FunctionTemplate::New(p_isolate, JsPlotsWriterStaggeredFile::writePlots));
	tpl->PrototypeTemplate()->Set(v8::String::NewFromUtf8(p_isolate, "isClosed"), v8::FunctionTemplate::New(p_isolate, JsPlotsWriterStaggeredFile::isClosed));
	tpl->PrototypeTemplate()->Set(v8::String::NewFromUtf8(p_isolate, "close"), v8::FunctionTemplate::New(p_isolate, JsPlotsWriterStaggeredFile::close));

	s_constructor.Reset(p_isolate, tpl->GetFunction());
}

v8::Handle<v8::Value> JsPlotsWriterStaggeredFile::New(v8::Isolate* p_isolate, std::unique_ptr<cryo::burstMine::plugins::native::PlotsWriterStaggeredFile>& p_plotsWriter) {
	v8::EscapableHandleScope scope(p_isolate);

	v8::Local<v8::Function> constructor = v8::Local<v8::Function>::New(p_isolate, s_constructor);
	v8::Local<v8::Object> obj = constructor->NewInstance();
	obj->SetInternalField(0, v8::External::New(p_isolate, p_plotsWriter.release()));

	return scope.Escape(obj);
}

void JsPlotsWriterStaggeredFile::unwrap(const v8::PropertyCallbackInfo<v8::Value>& p_info, std::function<void(cryo::burstMine::plugins::native::PlotsWriterStaggeredFile*)> p_functor) {
	v8::HandleScope scope(p_info.GetIsolate());
	v8::Local<v8::Object> self = p_info.Holder();
	v8::Local<v8::External> ext = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	if(ext->Value() == 0) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Plots writer closed")));
		return;
	}

	p_functor(static_cast<cryo::burstMine::plugins::native::PlotsWriterStaggeredFile*>(ext->Value()));
}

void JsPlotsWriterStaggeredFile::unwrap(const v8::FunctionCallbackInfo<v8::Value>& p_info, std::function<void(cryo::burstMine::plugins::native::PlotsWriterStaggeredFile*)> p_functor) {
	v8::HandleScope scope(p_info.GetIsolate());
	v8::Local<v8::Object> self = p_info.This();
	v8::Local<v8::External> ext = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	if(ext->Value() == 0) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Plots writer closed")));
		return;
	}

	p_functor(static_cast<cryo::burstMine::plugins::native::PlotsWriterStaggeredFile*>(ext->Value()));
}

void JsPlotsWriterStaggeredFile::getStaggerSize(v8::Local<v8::String> p_property, const v8::PropertyCallbackInfo<v8::Value>& p_info) {
	return unwrap(p_info, [&](cryo::burstMine::plugins::native::PlotsWriterStaggeredFile* p_plotsWriter) {
		return p_info.GetReturnValue().Set(v8::Number::New(p_info.GetIsolate(), static_cast<double>(p_plotsWriter->getStaggerSize())));
	});
}

void JsPlotsWriterStaggeredFile::getPath(v8::Local<v8::String> p_property, const v8::PropertyCallbackInfo<v8::Value>& p_info) {
	return unwrap(p_info, [&](cryo::burstMine::plugins::native::PlotsWriterStaggeredFile* p_plotsWriter) {
		return p_info.GetReturnValue().Set(v8::String::NewFromUtf8(p_info.GetIsolate(), p_plotsWriter->getPath().c_str()));
	});
}

void JsPlotsWriterStaggeredFile::writePlots(const v8::FunctionCallbackInfo<v8::Value>& p_info) {
	v8::HandleScope scope(p_info.GetIsolate());

	if(p_info.Length() < 1) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Missing [buffer] parameter")));
		return;
	} else if(p_info.Length() < 2) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Missing [size] parameter")));
		return;
	} else if(p_info.Length() < 3) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Missing [callback] parameter")));
		return;
	} else if(!cryo::burstMine::js::JsScoopsBuffer::HasInstance(p_info.GetIsolate(), p_info[0]->ToObject())) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Invalid [buffer] parameter")));
		return;
	} else if(!p_info[1]->IsNumber()) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Invalid [size] parameter")));
		return;
	} else if(!p_info[2]->IsFunction()) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Invalid [callback] parameter")));
		return;
	}

	std::unique_ptr<cryo::burstMine::plugins::native::js::impl::AsyncDataWritePlots> asyncData(new cryo::burstMine::plugins::native::js::impl::AsyncDataWritePlots);
	asyncData->isolate = p_info.GetIsolate();
	asyncData->callback.Reset(p_info.GetIsolate(), v8::Local<v8::Function>::Cast(p_info[2]));

	try {
		asyncData->buffer = cryo::burstMine::js::JsScoopsBuffer::FromObject(p_info.GetIsolate(), p_info[0]->ToObject());
		asyncData->size = static_cast<std::size_t>(p_info[1]->ToNumber()->Value());
	} catch(const std::exception& p_ex) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), p_ex.what())));
		return;
	}

	return unwrap(p_info, [&](cryo::burstMine::plugins::native::PlotsWriterStaggeredFile* p_plotsWriter) {
		asyncData->plotsWriter = p_plotsWriter;

		uv_work_t* work = new uv_work_t;
		work->data = static_cast<void*>(asyncData.release());

		uv_queue_work(
			uv_default_loop(),
			work,
			[](uv_work_t* p_work) {
				cryo::burstMine::plugins::native::js::impl::AsyncDataWritePlots* asyncData = static_cast<cryo::burstMine::plugins::native::js::impl::AsyncDataWritePlots*>(p_work->data);

				try {
					asyncData->plotsWriter->writePlots(
						*asyncData->buffer,
						asyncData->size
					);
				} catch(const std::exception&) {
					asyncData->exception = std::current_exception();
				}
			},
			[](uv_work_t* p_work, int p_status) {
				cryo::burstMine::plugins::native::js::impl::AsyncDataWritePlots* asyncData = static_cast<cryo::burstMine::plugins::native::js::impl::AsyncDataWritePlots*>(p_work->data);

				asyncData->triggerCallback({});
				delete asyncData;
				delete p_work;
			}
		);
	});
}

void JsPlotsWriterStaggeredFile::isClosed(const v8::FunctionCallbackInfo<v8::Value>& p_info) {
	v8::EscapableHandleScope scope(p_info.GetIsolate());
	v8::Local<v8::Object> self = p_info.This();
	v8::Local<v8::External> ext = v8::Local<v8::External>::Cast(self->GetInternalField(0));

	v8::Local<v8::Boolean> closed = v8::Boolean::New(p_info.GetIsolate(), ext->Value() != 0);
	p_info.GetReturnValue().Set(scope.Escape(closed));
}

void JsPlotsWriterStaggeredFile::close(const v8::FunctionCallbackInfo<v8::Value>& p_info) {
	v8::HandleScope scope(p_info.GetIsolate());

	if(p_info.Length() < 1) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Missing [callback] parameter")));
		return;
	} else if(!p_info[0]->IsFunction()) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Invalid [callback] parameter")));
		return;
	}

	std::unique_ptr<cryo::burstMine::plugins::native::js::impl::AsyncDataClose> asyncData(new cryo::burstMine::plugins::native::js::impl::AsyncDataClose);
	asyncData->isolate = p_info.GetIsolate();
	asyncData->callback.Reset(p_info.GetIsolate(), v8::Local<v8::Function>::Cast(p_info[0]));
	asyncData->self.Reset(p_info.GetIsolate(), p_info.This());

	return unwrap(p_info, [&](cryo::burstMine::plugins::native::PlotsWriterStaggeredFile* p_plotsWriter) {
		asyncData->plotsWriter = p_plotsWriter;

		uv_work_t* work = new uv_work_t;
		work->data = static_cast<void*>(asyncData.release());

		uv_queue_work(
			uv_default_loop(),
			work,
			[](uv_work_t* p_work) {
				cryo::burstMine::plugins::native::js::impl::AsyncDataClose* asyncData = static_cast<cryo::burstMine::plugins::native::js::impl::AsyncDataClose*>(p_work->data);

				try {
					delete asyncData->plotsWriter;

					v8::Local<v8::Object> self = v8::Local<v8::Object>::New(asyncData->isolate, asyncData->self);
					self->SetInternalField(0, v8::Undefined(asyncData->isolate));
				} catch(const std::exception&) {
					asyncData->exception = std::current_exception();
				}
			},
			[](uv_work_t* p_work, int p_status) {
				cryo::burstMine::plugins::native::js::impl::AsyncDataClose* asyncData = static_cast<cryo::burstMine::plugins::native::js::impl::AsyncDataClose*>(p_work->data);

				asyncData->triggerCallback({});
				delete asyncData;
				delete p_work;
			}
		);
	});
}

}}}}}
