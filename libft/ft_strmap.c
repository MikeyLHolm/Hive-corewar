/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 12:38:41 by mlindhol          #+#    #+#             */
/*   Updated: 2019/11/27 13:52:37 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	char			*new;
	unsigned int	i;

	if (!s || !(new = ft_strdup(s)))
		return (NULL);
	i = 0;
	while (new[i] != '\0')
	{
		new[i] = f(new[i]);
		i++;
	}
	return (new);
}
