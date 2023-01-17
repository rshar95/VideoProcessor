/**
 * Front-end module
 * This module is responsible for providing a user interface for interacting with the video processing system
 * @param app: AngularJS module
 * @param myCtrl: AngularJS controller
 * @param $scope: AngularJS scope object
 * @param $http: AngularJS http service for making HTTP requests
 * @param videos: array of videos
 * @param getVideos: function for getting list of videos
 * @return: void
 */

// app.js
var app = angular.module('myApp', []);
app.controller('myCtrl', function($scope, $http) {
    $scope.videos = [];
    $scope.getVideos = function() {
        $http({
            method: 'GET',
            url: '/videos'
        }).then(function successCallback(response) {
            $scope.videos = response.data;
        }, function errorCallback(response) {
            console.log('Error getting videos');
        });
    };
    $scope.getVideos();
});

// index.html
<div ng-app="myApp" ng-controller="myCtrl">
    <table>
        <tr ng-repeat="video in videos">
            <td>{{ video.name }}</td>
            <td>{{ video.fps }}</td>
            <td>{{ video.width }}x{{ video.height }}</td>
        </tr>
    </table>
</div>
