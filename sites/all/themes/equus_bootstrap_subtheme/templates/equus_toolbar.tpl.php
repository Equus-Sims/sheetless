<div class="toolbar">
	<ul>
		<?php if ($logged_in): ?>

		<li><?php print $name; ?>
			<ul>
				<li><?php print l('Dashboard', 'dashboard'); ?></li>
				<li><?php print l('View Profile', "user/$uid/profile"); ?></li>
				<li><?php print l('Edit Profile', "user/$uid/edit"); ?></li>
				<li><?php print l('Logout', 'user/logout'); ?></li>
			</ul>
		</li>
		

		<li><?php print l('Messages', 'messages'); ?></li>
		<li><?php print l('Notifications', 'dashboard'); ?></li>
		<li>Net Worth: <?php print $net_worth; ?>
			<ul>
				<?php foreach($orgs as $org): ?>
				<li><?php 
					print l($org['name'], $org['path']);
					print ": ";
					print l($org['bank_balance'], $org['bank_transactions_path']);
				?></li>
				<?php endforeach; ?>
			</ul>
		</li>
		<li>Credits: <?php print $total_credits; ?>
			<ul>
				<li>Regular Horse Credit: <?php print l($regular_credit, 'dashboard'); ?></li>
				<li>Rare Horse Credit: <?php print l($rare_credit, 'dashboard'); ?></li>
				<li>Organization Credit: <?php print l($org_credit, 'dashboard'); ?></li>
			</ul>
		</li>
		<li>MP: 0</li>
		<li>Create a...
			<ul>
				<li><?php print l('Blog Entry', 'node/add/blog'); ?></li>
				<li><?php print l('Horse', current_path(), array('fragment' => 'overlay=node/add/horse')); ?></li>
				<li><?php print l('Organization', 'node/add/organization'); ?></li>
				<li><?php print l('Property', 'node/add/property'); ?></li>
				<li><?php print l('Show', 'node/add/show'); ?></li>
				<li><?php print l('Sale', 'node/add/equus-sale'); ?></li>
				<li><?php print l('Transaction', 'node/add/transaction'); ?></li>
			</ul>
		</li>
		<?php else: ?>
			<li><?php print l('Login', 'user/login'); ?></li>
			<li><?php print l('Register', 'user/register'); ?></li>
		<?php endif; ?>
		<li><?php print render($search_box); ?></li>
	</ul>
</div>