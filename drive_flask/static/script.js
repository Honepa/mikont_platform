var state = {
    forward : 0,
    back    : 0,
    right   : 0,
    left    : 0,
    on_line : 0,
    stop    : 0,
    speed   : 0,
    dir     : 0,
};
    
let timer_id = setInterval(fl_drive, 500);
function fl_drive(){
    console.log(state),
    $.ajax({
        url: '/state',
        type: "POST",
        data: state,
        success: function(responce) {
            $('pre').text(responce);
            //console.log($('log'));
        },
        error: function(error) {
            console.log(error);
        }
    });
};    

$(window).on('mouseup', function(){
    state.forward = 0;
    state.back    = 0;
    state.left    = 0;
    state.right   = 0;
});
$("#forward").on("touchstart", function(){
    state.forward = 1;
    state.back    = 0;
    state.left    = 0;
    state.right   = 0;
    state.on_line = 0;
    state.stop    = 0;

});
$("#forward").on("mousedown", function(){
    state.forward = 1;
    state.back    = 0;
    state.left    = 0;
    state.right   = 0;
    state.on_line = 0;
    state.stop    = 0;

});/*
document.getElementById("back").addEventListener("mousedown", function(){
    state.forward = 0;
    state.back    = 1;
    state.left    = 0;
    state.right   = 0;
    state.on_line = 0;
    state.stop    = 0;
});
document.getElementById("left").addEventListener("mousedown", function(){
    state.forward = 0;
    state.back    = 0;
    state.left    = 1;
    state.right   = 0;
    state.on_line = 0;
    state.stop    = 0;
});
document.getElementById("right").addEventListener("mousedown", function(){
    state.forward = 0;
    state.back    = 0;
    state.left    = 0;
    state.right   = 1;
    state.on_line = 0;
    state.stop    = 0;
});
*/
document.getElementById("on_line").addEventListener("mousedown", function(){
    state.forward = 0;
    state.back    = 0;
    state.left    = 0;
    state.right   = 0;
    state.on_line = 1;
    state.stop    = 0;
});
$("#on_line").on("touchstart", function(){
    state.forward = 0;
    state.back    = 0;
    state.left    = 0;
    state.right   = 0;
    state.on_line = 1;
    state.stop    = 0;

});
document.getElementById("stop").addEventListener("mousedown", function(){
    state.forward = 0;
    state.back    = 0;
    state.left    = 0;
    state.right   = 0;
    state.on_line = 0;
    state.stop    = 1;
});


function change_speed(new_speed) {
    state.speed = new_speed;
};
function change_dir(new_dir){
    state.dir = new_dir;
};

function in_zero() {
    document.getElementById("dir").value = 0;
    state.dir     = 0;
};