#ifndef MATERIASOURCE_HPP
#define MATERIASOURCE_HPP

#include "IMateriaSource.hpp"
#include "AMateria.hpp"
#include <string>

class MateriaSource : public IMateriaSource {
private:
    AMateria* prototypes[4];

public:
    MateriaSource();
    MateriaSource(MateriaSource const & other);
    virtual ~MateriaSource();

    MateriaSource & operator=(MateriaSource const & other);

    virtual void learnMateria(AMateria* m);
    virtual AMateria* createMateria(std::string const & type);
};

#endif  // MATERIASOURCE_HPP
