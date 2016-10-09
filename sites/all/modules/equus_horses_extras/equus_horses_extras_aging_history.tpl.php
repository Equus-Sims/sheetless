<?php if ($has_aging_details): ?>
    <table>
        <tr>
            <th>Lifestage</th>
            <th>Speed</th>
            <th>Start Date</th>
            <th>End Date</th>
        </tr>
        <?php foreach ($aging_details as $detail): ?>
        <tr>
            <td><?php print $detail->lifestage; ?></td>
            <td><?php print $detail->speed; ?>x</td>
            <td><?php print (new DateTime($detail->start))->format('Y-m-d'); ?></td>
            <td><?php print (new DateTime($detail->end))->format('Y-m-d'); ?></td>
        </tr>
        <?php endforeach; ?>
    </table>
<?php else: ?>
    <p>This horse has no aging detail history to display.</p>
<?php endif; ?>