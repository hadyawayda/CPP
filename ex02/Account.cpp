/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   account.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 20:55:14 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/17 20:55:14 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Account.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>

// ──────────────────────────────────────────────────────────────────────────────
// Static member definitions
// ──────────────────────────────────────────────────────────────────────────────
int Account::_nbAccounts        = 0;
int Account::_totalAmount       = 0;
int Account::_totalNbDeposits   = 0;
int Account::_totalNbWithdrawals= 0;

// ──────────────────────────────────────────────────────────────────────────────
// Timestamp helper: prints “[YYYYMMDD_HHMMSS] ”
// ──────────────────────────────────────────────────────────────────────────────
void Account::_displayTimestamp(void)
{
    std::time_t now = std::time(nullptr);
    std::tm     *t  = std::localtime(&now);
    std::cout << '['
              << (t->tm_year + 1900)
              << std::setw(2) << std::setfill('0') << (t->tm_mon + 1)
              << std::setw(2) << std::setfill('0') <<  t->tm_mday << '_'
              << std::setw(2) << std::setfill('0') <<  t->tm_hour
              << std::setw(2) << std::setfill('0') <<  t->tm_min
              << std::setw(2) << std::setfill('0') <<  t->tm_sec
              << "] ";
}

// ──────────────────────────────────────────────────────────────────────────────
// Constructor / Destructor
// ──────────────────────────────────────────────────────────────────────────────
Account::Account(int initial_deposit)
: _accountIndex(_nbAccounts),
  _amount(initial_deposit),
  _nbDeposits(0),
  _nbWithdrawals(0)
{
    ++_nbAccounts;
    _totalAmount += _amount;
    _displayTimestamp();
    std::cout << "index:"  << _accountIndex
              << ";amount:" << _amount
              << ";created" << std::endl;
}

Account::~Account(void)
{
    _displayTimestamp();
    std::cout << "index:"  << _accountIndex
              << ";amount:" << _amount
              << ";closed"  << std::endl;
}

// ──────────────────────────────────────────────────────────────────────────────
// Static accessors & info dump
// ──────────────────────────────────────────────────────────────────────────────
int  Account::getNbAccounts(void)    { return _nbAccounts; }
int  Account::getTotalAmount(void)   { return _totalAmount; }
int  Account::getNbDeposits(void)    { return _totalNbDeposits; }
int  Account::getNbWithdrawals(void) { return _totalNbWithdrawals; }

void Account::displayAccountsInfos(void)
{
    _displayTimestamp();
    std::cout << "accounts:"    << _nbAccounts
              << ";total:"      << _totalAmount
              << ";deposits:"   << _totalNbDeposits
              << ";withdrawals:"<< _totalNbWithdrawals
              << std::endl;
}

// ──────────────────────────────────────────────────────────────────────────────
// Instance methods
// ──────────────────────────────────────────────────────────────────────────────
void Account::displayStatus(void) const
{
    _displayTimestamp();
    std::cout << "index:"       << _accountIndex
              << ";amount:"     << _amount
              << ";deposits:"   << _nbDeposits
              << ";withdrawals:"<< _nbWithdrawals
              << std::endl;
}

void Account::makeDeposit(int deposit)
{
    _displayTimestamp();
    std::cout << "index:"     << _accountIndex
              << ";p_amount:" << _amount
              << ";deposit:"  << deposit;
    _amount            += deposit;
    _totalAmount       += deposit;
    ++_nbDeposits;
    ++_totalNbDeposits;
    std::cout << ";amount:"       << _amount
              << ";nb_deposits:" << _nbDeposits
              << std::endl;
}

bool Account::makeWithdrawal(int withdrawal)
{
    _displayTimestamp();
    std::cout << "index:"     << _accountIndex
              << ";p_amount:" << _amount
              << ";withdrawal:";
    if (withdrawal > _amount)
    {
        std::cout << "refused" << std::endl;
        return false;
    }
    std::cout << withdrawal;
    _amount             -= withdrawal;
    _totalAmount        -= withdrawal;
    ++_nbWithdrawals;
    ++_totalNbWithdrawals;
    std::cout << ";amount:"       << _amount
              << ";nb_withdrawals:" << _nbWithdrawals
              << std::endl;
    return true;
}

int Account::checkAmount(void) const
{
    return _amount;
}
