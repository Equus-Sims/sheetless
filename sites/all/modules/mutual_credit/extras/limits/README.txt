The limits module extends the currency definition with balance limits.
A transaction cluster will fail validation if it would take any of the participants beyond their balance limits in any currency.
Balance limits are not stored, but always calculated on the fly.
2 blocks are provided, one to show the balance limits and one for the trading limits, meaning how much the user can earn or spend, within the balance limits, given their current balance.
E.g. so if the balance limits are +- 100 and your balance is +90 you trading limits will be: earning limit 10, spending limit 190
The limit settings allow several kinds of override and there is an API for providing new ways of calculating.
