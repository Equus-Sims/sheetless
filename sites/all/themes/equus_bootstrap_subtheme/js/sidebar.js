(function($, Drupal, window, document, undefined) {
    $(document).ready(function() {
        
        console.log('bar');
        $('.sidebar-credit-item').click(function() {
            $('.sidebar-credit-item + .expand, .sidebar-credit-item .sidebar-chevron').toggle();
        });

        $('.sidebar-net-worth-item').click(function() {
            $('.sidebar-net-worth-item + .expand, .sidebar-net-worth-item .sidebar-chevron').toggle();
        });

        $('.sidebar-create-item').click(function() {
            $('.sidebar-create-item + .expand, .sidebar-create-item .sidebar-chevron').toggle();
        });
    });
    
    //document.querySelector('.sidebar-credit-item').onclick = function() {jQuery('.expand').toggle();};
}(jQuery, Drupal, this, document, undefined));