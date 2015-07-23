'use strict';

/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

context.addController('burstMine-plugin-native-plotsWrapper-staggeredFile-edit', [
	'$scope', 'component',
	function($scope, p_component) {
		$scope.component = p_component;
		$scope.data = p_component.params;
		$scope.data.staggerSize = 1024;
		$scope.data.path = '';

		$scope.correctNumber = function() {
			if($scope.component.number < $scope.data.staggerSize) {
				$scope.component.number = $scope.data.staggerSize;
				return;
			}

			var mod = $scope.component.number % $scope.data.staggerSize;
			if(mod > $scope.data.staggerSize / 2) {
					$scope.component.number += $scope.data.staggerSize - mod;
			} else if(mod > 0) {
				$scope.component.number -= mod;
			}
		};

		$scope.$watch('component.number', function(p_newValue, p_oldValue) {
			var diff = p_newValue - p_oldValue;
			switch(diff) {
				case 0: return;
				case 1: $scope.component.number += $scope.data.staggerSize; break;
				case -1: $scope.component.number -= $scope.data.staggerSize; break;
			}

			$scope.correctNumber();
		});

		$scope.correctNumber();
	}
]);