.pragma library
var saved_username;
var token;
var role;
var state = "Off";
const url_base = 'http://10.0.1.119:8080/'

var last_reachable = 0;
var last_heartbeat = 0;

function is_admin() { return (role == 'ROLE_ADMIN'); }
function is_police() { return (role == 'ROLE_ADMIN' || role == 'ROLE_POLICE'); }
function is_off() { return (state == "Off"); }
function is_reachable() { return (new Date()).getTime() - last_reachable < 3000; }
function is_device_reachable() { return (new Date()).getTime() - last_heartbeat < 3000; }
function get_username() { return saved_username; }

function rest_request(url_extension, request_type, body, body_type, onSuccess, onError, use_token) {
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.onreadystatechange = function() {
        if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
            onSuccess(xmlHttp.responseText);
        else if (xmlHttp.readyState == 4) onError(xmlHttp.status + ': ' + xmlHttp.responseText)
    }
    xmlHttp.open(request_type, url_base + url_extension, true);
    xmlHttp.setRequestHeader("Content-Type", body_type);
    if(use_token) xmlHttp.setRequestHeader('Authorization', 'Bearer ' + token);
    xmlHttp.send(body);
}

function init(onSuccess, onError) {
    rest_request("users/init", "POST", null, "text/plain", onSuccess, onError);
}


function get_heartbeat() {
    rest_request("trafficlight/lastheartbeat", "GET", null, "text/plain", (text) => {last_reachable = (new Date()).getTime(); last_heartbeat = +(text)}, ()=>{}, true);
}

function sign_in(username, password, onSuccess, onError) {
    saved_username = username;
    rest_request("users/signin", "POST", JSON.stringify({username, password}), "application/json", (text) => {token = JSON.parse(text).accessToken; role = JSON.parse(text).roles[0]; onSuccess(text)}, onError, false);
}

function sign_out() {
    saved_username = '';
    token = '';
    role = '';
    state = "Off";
    last_heartbeat = 0;
    last_reachable = 0;
}

function modify_user(user, onSuccess, onError) {
    rest_request("users/" + user.username, "PUT", JSON.stringify(user), "application/json", onSuccess, onError, true);
}

function delete_user(username, onSuccess, onError) {
    rest_request("users/" + username, "DELETE", null, "text/plain", onSuccess, onError, true);
}

function new_user(username, password, role, onSuccess, onError) {
    rest_request("users/", "POST", JSON.stringify({username, password, role}), "application/json", onSuccess, onError, true);
}

function get_all_logs(onSuccess, onError) {
    rest_request("logs/all", "GET", null, "text/plain", onSuccess, onError, true);
}

function get_filtered_logs(since, minLogLevel, onSuccess, onError) {
    rest_request("logs/filtered?since=" + since + "&minLogLevel=" + minLogLevel, "GET", null, "text/plain", onSuccess, onError, true);
}

function request_test(tests, onSuccess, onError) {
    rest_request("trafficlight/test", "POST", JSON.stringify(tests), "application/json", onSuccess, onError, true);
}

function send_police(onSuccess, onError) {
    rest_request("trafficlight/inputs/police", "POST", null, "text/plain", onSuccess, onError, true);

}

function send_switch(sw, onSuccess, onError) {
    rest_request("trafficlight/inputs/switch?state=" + sw, "POST", null, "text/plain", onSuccess, onError, true);

}

function get_test_status(id, onSuccess, onError) {
    rest_request("trafficlight/test/" + id, "GET", null, "text/plain", onSuccess, onError, true);
}

function get_state(onSuccess, onError) {
    rest_request("trafficlight/state", "GET", null, "text/plain", (s) => {state = s; onSuccess(s)}, onError, true);
}


function get_signal(onSuccess, onError) {
    rest_request("trafficlight/sensors/signal", "GET", null, "text/plain", onSuccess, onError, true);
}

function get_stopped(onSuccess, onError) {
    rest_request("trafficlight/sensors/stopped", "GET", null, "text/plain", onSuccess, onError, true);
}

function get_time(timing, onSuccess, onError) {
    rest_request("trafficlight/times/" + timing, "GET", null, "text/plain", onSuccess, onError, true);
}

function set_state(state, onSuccess, onError) {
    rest_request("trafficlight/state", "PUT", state, "text/plain", onSuccess, onError, true);
}

function set_time(timing, value, onSuccess, onError) {
    rest_request("trafficlight/times/" + timing, "PUT", value, "text/plain", onSuccess, onError, true);
}

function get_messages_all(onSuccess, onError) {
    rest_request("messages/all", "GET", null, "text/plain", onSuccess, onError, true);
}

function get_messages_filtered(since, onSuccess, onError) {
    rest_request("messages/filtered?since=" + since, "GET", null, "text/plain", onSuccess, onError, true);
}

function post_message(username, message, role, onSuccess, onError) {
    rest_request("messages/", "POST", JSON.stringify({username, message, role}), "application/json", onSuccess, onError, true);
}

