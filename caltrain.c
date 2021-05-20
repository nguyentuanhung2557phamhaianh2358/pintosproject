#include "pintos_thread.h"
#include <stdio.h>
struct station
{
	int ghe_trong;
	int hk_doi_o_ga;
	int hk_len_tau;
	struct lock *khoa;
	struct condition *Tat_ca_hk_len_tau;
	struct condition *Tau_den;
};
void station_init(struct station *x)
{
	x->ghe_trong = 0;
	x->hk_doi_o_ga = 0;
	x->hk_len_tau = 0;
	x->khoa = malloc(sizeof(struct lock));
	lock_init(x->khoa);
	x->Tat_ca_hk_len_tau = malloc(sizeof(struct condition));
	cond_init(x->Tat_ca_hk_len_tau);
	x->Tau_den = malloc(sizeof(struct condition));
	cond_init(x->Tau_den);
};
void station_load_train(struct station *station, int count)
{
	lock_acquire(station->khoa);
	station->ghe_trong = count;

	while ((station->hk_doi_o_ga > 0) && (station->ghe_trong > 0))
	{
		cond_broadcast(station->Tau_den, station->khoa);
		cond_wait(station->Tat_ca_hk_len_tau, station->khoa);
	}

	station->ghe_trong = 0;
	lock_release(station->khoa);
}

void station_wait_for_train(struct station *station)
{
	lock_acquire(station->khoa);
	station->hk_doi_o_ga++;
	while (station->hk_len_tau == station->ghe_trong || station->ghe_trong == 0)
	{
		cond_wait(station->Tau_den, station->khoa);
	}
	station->hk_doi_o_ga--;
	station->hk_len_tau++;
	lock_release(station->khoa);
}
void station_on_board(struct station *station)
{
	lock_acquire(station->khoa);
	station->hk_len_tau--;
	station->ghe_trong--;
	if ((station->ghe_trong == 0) || (station->hk_len_tau == 0))
	{
		cond_signal(station->Tat_ca_hk_len_tau, station->khoa);
	}

	lock_release(station->khoa);
}
