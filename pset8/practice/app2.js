function quote() {
    var url = '/quote?symbol=' + $('#symbol').val();

    $.getJSON(url, function(data) {
        $('#quote').html('A share of ' + data.name + ' costs $' + data.price + '.');
        $('#symbol').val('');
    });
}