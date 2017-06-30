(function($, Drupal, window, document, undefined) {
    $(document).ready(function() {

        

        var expandSidebar = function() {
            console.log( 'expanding the sidebar' );
            $('#sidebar').removeClass('collapse');
            $('#sidebar, #sidebar .search-block .input-group input#edit-keys, #sidebar .search-block .input-group input#edit-keys--2, #sidebar #account, #sidebar #account img').addClass('expand');
            $('.sidebar-item').removeClass('item-hide');
            $('#content-area').removeClass('sidebar-collapse');
            $('.sidebar-menu-button').toggle();
            $('.sidebar-close-button').toggle();
            $('#sidebar #sidebar-menu ul#login-menu').addClass('expand');
        };

        var collapseSidebar = function() {
            if ($('.sidebar-credit-item.open-subitem').length) {
                $('.sidebar-credit-item + .expand, .sidebar-net-worth-item .sidebar-chevron').toggle();
                $('.sidebar-credit-item').toggleClass('open-subitem');
            }

            if ($('.sidebar-net-worth-item.open-subitem').length) {
                $('.sidebar-net-worth-item + .expand, .sidebar-net-worth-item .sidebar-chevron').toggle();
                $('.sidebar-net-worth-item').toggleClass('open-subitem');
            }

            if ($('.sidebar-create-item.open-subitem').length) {
                $('.sidebar-create-item + .expand, .sidebar-net-worth-item .sidebar-chevron').toggle();
                $('.sidebar-create-item').toggleClass('open-subitem');
            }

            console.log( 'collapsing the sidebar' );
            $('#sidebar').addClass('collapse');
            $('#sidebar, #sidebar .search-block .input-group input#edit-keys, #sidebar .search-block .input-group input#edit-keys--2, #sidebar #account, #sidebar #account img').removeClass('expand');
            $('.sidebar-item').addClass('item-hide');
            $('#content-area').addClass('sidebar-collapse');
            $('.sidebar-close-button').toggle();
            $('.sidebar-menu-button').toggle();
            $('#banking-info').hide();
            $('#sidebar #sidebar-menu ul#login-menu').removeClass('expand');

        };

        $( '.sidebar-menu-button' ).on( 'click', expandSidebar );
        $( '.sidebar-close-button' ).on( 'click', collapseSidebar );

        $('.sidebar-credit-item').click(function() {
            if ($("#sidebar.collapse").length) {
                expandSidebar();
            }
            $('.sidebar-credit-item + .expand, .sidebar-credit-item .sidebar-chevron').toggle();
            $('.sidebar-credit-item').toggleClass('open-subitem');
        });

        $('.sidebar-net-worth-item').click(function() {
            if ($("#sidebar.collapse").length) {
                expandSidebar();
            }
            $('.sidebar-net-worth-item + .expand, .sidebar-net-worth-item .sidebar-chevron').toggle();
            $('.sidebar-net-worth-item').toggleClass('open-subitem');
        });

        $('.sidebar-create-item').click(function() {
            if ($("#sidebar.collapse").length) {
                expandSidebar();
            }
            $('.sidebar-create-item + .expand, .sidebar-create-item .sidebar-chevron').toggle();
            $('.sidebar-create-item').toggleClass('open-subitem');
        });

        $('button.search-button').click(function(e) {
            if ($("#sidebar.collapse").length) {
                e.preventDefault();
                expandSidebar();
            }
        });
    });
    
    //document.querySelector('.sidebar-credit-item').onclick = function() {jQuery('.expand').toggle();};
}(jQuery, Drupal, this, document, undefined));