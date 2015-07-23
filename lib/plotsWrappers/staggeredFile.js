'use strict';

/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

var lib = {
	deps:{
		q:require('q'),
		joi:require('joi')
	},
	ErrorRest:require('../../../../ErrorRest')
};

var bindings = require('../../bin/burstMine-plugin-native');

function PlotsWrapperStaggeredFile(p_native, p_plots) {
	Object.defineProperty(this, '_native', {value:p_native});
	Object.defineProperty(this, 'plots', {enumerable:true, value:p_plots});
};

PlotsWrapperStaggeredFile.prototype.writePlots = function(p_buffer, p_size) {
	return lib.deps.q.ninvoke(this._native, 'writePlots', p_buffer, p_size);
};

PlotsWrapperStaggeredFile.prototype.close = function() {
	return lib.deps.q.ninvoke(this._native, 'close');
};

module.exports.parseParams = function(p_plots, p_params) {
	var result = lib.deps.joi.validate(p_params, {
		staggerSize:lib.deps.joi.number().required().integer().min(1),
		path:lib.deps.joi.string().required().min(1).max(255)
	});

	if(result.error) {
		throw new lib.ErrorRest(409, 'validation', 'Invalid input parameters', result.error.details);
	}

	var postResult = lib.deps.joi.validate(p_plots, {
		number:lib.deps.joi.number().multiple(result.value.staggerSize)
	}, {
		abortEarly:false,
		allowUnknown:true,
		presence:'required'
	});

	if(postResult.error) {
		throw new lib.ErrorRest(409, 'validation', 'Invalid input parameters', postResult.error.details);
	}

	return result.value;
};

module.exports.createWriter = function(p_context, p_plots) {
	return lib.deps.q.nfcall(bindings.createPlotsWriter, p_plots)
	.then(function(p_plotsWriter) {
		return new PlotsWrapperStaggeredFile(p_plotsWriter, p_plots);
	});
};