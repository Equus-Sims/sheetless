<?php
/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * The doctype, html, head and body tags are not in this template. Instead they
 * can be found in the html.tpl.php template in this directory.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/bartik.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['header']: Items for the header region.
 * - $page['footer']: Items for the footer region.
 *
 * @see bootstrap_preprocess_page()
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see bootstrap_process_page()
 * @see template_process()
 * @see html.tpl.php
 *
 * @ingroup themeable
 */
?>
<script>

  

</script>

<div class="main-container container">
    <header id="top-bar" role="banner"> <!-- /#top-bar -->
        <h1 id="site-logo">
            <img src="<?php print $logo; ?>">
        </h1>
        <h1 id="site-name"><?php print l('Equus','',array('absolute'=>true)); ?></h1>
        <header id="navbar" role="navigation">
            <nav id="main-menu">
                <?php if (!empty($main_menu)): ?>
                    <?php print render($main_menu); ?>
                <?php endif; ?>
            </nav>
        </header>
    </header>

    <div id="content-container">
        <div id="sidebar">
            <?php if ($logged_in): ?>
                <nav id="account">
                    <?php print render($user_picture) . l($name, "user/$uid/profile"); ?>
                </nav>
                <nav id="sidebar-menu">
                    <ul>
                        <li><?php print l("$dashboard_icon Dashboard", 'dashboard', array('html' => TRUE)); ?></li>

                        <li><?php print l("$messages_icon Messages", 'messages', array('html' => TRUE)); ?></li>

                        <li><?php print l("$bell_icon Notifications", 'dashboard', array('html' => TRUE)); ?></li>

                        <li><div class="sidebar-credit-item"><?php print $credit_icon . "Credits: " . $total_credits . $chevron_up_icon . $chevron_down_icon; ?></div>
                            <ul id="credit-info" class="expand">
                                <li><span>Regular Horse Credit: <?php print l($regular_credit, 'dashboard'); ?></span></li>
                                <li><span>Organization Credit: <?php print l($org_credit, 'dashboard'); ?></span></li>
                            </ul>
                        </li>

                        <li><div class="sidebar-net-worth-item"><?php print $credit_card_icon . "Net Worth: " . $net_worth . $chevron_up_icon . $chevron_down_icon; ?></div>
                            <ul id="banking-info" class="expand">
                                <?php foreach($orgs as $org): ?>
                                    <li><span><?php
                                            print l($org['name'], $org['path'], array('attributes' => array('class' => array('org-name'))));
                                            print ": ";
                                            print l($org['bank_balance'], $org['bank_transactions_path']);
                                            ?></span></li>
                                <?php endforeach; ?>
                            </ul>
                        </li>

                        <li><div class="sidebar-create-item"><?php print $plus_circle_icon . "Create: " . $chevron_up_icon . $chevron_down_icon; ?></div>
                            <ul id="create_items" class="expand">
                                <li><?php print l('Blog Entry', 'node/add/blog'); ?></li>
                                <li><?php print l('Horse', current_path(), array('fragment' => 'overlay=node/add/horse')); ?></li>
                                <li><?php print l('Organization', 'node/add/organization'); ?></li>
                                <li><?php print l('Property', 'node/add/property'); ?></li>
                                <li><?php print l('Show', 'node/add/show'); ?></li>
                                <li><?php print l('Sale', 'node/add/equus-sale'); ?></li>
                                <li><?php print l('Transaction', 'node/add/transaction'); ?></li>
                            </ul>
                        </li>

                        <li><?php print l("$settings_icon Edit Profile", "user/$uid/edit", array('html' => TRUE)); ?></li>

                        <li><?php print l("$log_out_icon Logout", 'user/logout', array('html' => TRUE)); ?></li>
                    </ul>
                    <div class="search-block">
                        <?php print render($search_box); ?>
                    </div>
                </nav>
            <?php else: ?>
                <nav id="sidebar-menu">
                    <ul>
                        <li><?php print l("$log_in_icon Login", 'user/login', array('html' => TRUE)); ?></li>
                        <li><?php print l("$register_icon Register", 'user/register', array('html' => TRUE)); ?></li>
                    </ul>
                </nav>
            <?php endif; ?>
        </div>
        <div id="content-area">
            <div id="hero">
                <?php print render($page['header']); ?>
            </div>
            <div id="content">
                <?php print $messages; ?>
                <div id="content-with-sidebar">
                    <div>
                    <?php print render($page['content']); ?>
                    <?php print render($page['content_bottom']); ?>
                    </div>
                    <?php if (!empty($page['sidebar_small'])): ?>
                        <aside class="col-sm-3" role="complimentary">
                            <?php print render($page['sidebar_small']); ?>
                        </aside> <!-- /#sidebar-small -->
                    <?php endif; ?>
                </div>
            </div>
            <footer class="footer container">
                <?php print render($page['footer']); ?>
            </footer>
        </div>
    </div>
      <!-- content_bottom_region is being printed by the node.tpl.php file -->
  </div>
</div>
