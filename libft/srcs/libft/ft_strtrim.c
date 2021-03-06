/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 16:32:17 by mlindhol          #+#    #+#             */
/*   Updated: 2020/08/26 18:20:10 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	len;
	char			*str;
	char			*new;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	str = (char*)s;
	while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == ' ' ||
				str[len - 1] == '\t'))
		len--;
	i = 0;
	while (str[i] == '\n' || str[i] == ' ' || str[i] == '\t')
		i++;
	if (len < i)
		len = i;
	if (!(new = (char *)ft_memalloc(sizeof(char) * (len - i + 1))))
		return (NULL);
	j = 0;
	while (i < len)
		new[j++] = str[i++];
	new[j] = '\0';
	return (new);
}
