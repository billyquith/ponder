/****************************************************************************
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** CAMP is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** CAMP is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public License
** along with CAMP.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/


#ifndef CAMP_QT_QTFUNCTION_HPP
#define CAMP_QT_QTFUNCTION_HPP


#include <camp/qt/qthelper.hpp>
#include <camp/function.hpp>
#include <camp/userobject.hpp>
#include <camp/value.hpp>
#include <QMetaMethod>
#include <QVariant>
#include <QString>


namespace camp_ext
{
/**
 * \brief Specialization of camp::Function implemented using a Qt method
 *
 * This class is instanciated and returned by QtMapper<T>.
 *
 * \sa QtMapper
 */
template <typename T>
class QtFunction : public camp::Function
{
public:

    /**
     * \brief Construct the function from a QMetaMethod
     *
     * \param metaMethod Qt meta method
     */
    QtFunction(const QMetaMethod& metaMethod)
        : camp::Function(functionName(metaMethod), returnType(metaMethod), argTypes(metaMethod))
        , m_metaMethod(metaMethod)
    {
    }

    /**
     * \see Function::execute
     */
    virtual camp::Value execute(const camp::UserObject& object, const camp::Args& args) const
    {
        QVariant value(QtHelper::variantType(QMetaType::type(m_metaMethod.typeName())));
        QGenericReturnArgument ret(m_metaMethod.typeName(), value.data());

        switch (args.count())
        {
            case 0:
            {
                m_metaMethod.invoke(object.get<T*>(), Qt::DirectConnection, ret);
                break;
            }

            case 1:
            {
                QVariant arg1 = QtHelper::valueToVariant(args[0]);
                m_metaMethod.invoke(object.get<T*>(), Qt::DirectConnection, ret
                                  , QGenericArgument(arg1.typeName(), arg1.data()));
                break;
            }

            case 2:
            {
                QVariant arg1 = QtHelper::valueToVariant(args[0]);
                QVariant arg2 = QtHelper::valueToVariant(args[1]);
                m_metaMethod.invoke(object.get<T*>(), Qt::DirectConnection, ret
                                  , QGenericArgument(arg1.typeName(), arg1.data())
                                  , QGenericArgument(arg2.typeName(), arg2.data()));
                break;
            }

            case 3:
            {
                QVariant arg1 = QtHelper::valueToVariant(args[0]);
                QVariant arg2 = QtHelper::valueToVariant(args[1]);
                QVariant arg3 = QtHelper::valueToVariant(args[2]);
                m_metaMethod.invoke(object.get<T*>(), Qt::DirectConnection, ret
                                  , QGenericArgument(arg1.typeName(), arg1.data())
                                  , QGenericArgument(arg2.typeName(), arg2.data())
                                  , QGenericArgument(arg3.typeName(), arg3.data()));
                break;
            }

            case 4:
            {
                QVariant arg1 = QtHelper::valueToVariant(args[0]);
                QVariant arg2 = QtHelper::valueToVariant(args[1]);
                QVariant arg3 = QtHelper::valueToVariant(args[2]);
                QVariant arg4 = QtHelper::valueToVariant(args[3]);
                m_metaMethod.invoke(object.get<T*>(), Qt::DirectConnection, ret
                                  , QGenericArgument(arg1.typeName(), arg1.data())
                                  , QGenericArgument(arg2.typeName(), arg2.data())
                                  , QGenericArgument(arg3.typeName(), arg3.data())
                                  , QGenericArgument(arg4.typeName(), arg4.data()));
                break;
            }

            case 5:
            {
                QVariant arg1 = QtHelper::valueToVariant(args[0]);
                QVariant arg2 = QtHelper::valueToVariant(args[1]);
                QVariant arg3 = QtHelper::valueToVariant(args[2]);
                QVariant arg4 = QtHelper::valueToVariant(args[3]);
                QVariant arg5 = QtHelper::valueToVariant(args[4]);
                m_metaMethod.invoke(object.get<T*>(), Qt::DirectConnection, ret
                                  , QGenericArgument(arg1.typeName(), arg1.data())
                                  , QGenericArgument(arg2.typeName(), arg2.data())
                                  , QGenericArgument(arg3.typeName(), arg3.data())
                                  , QGenericArgument(arg4.typeName(), arg4.data())
                                  , QGenericArgument(arg5.typeName(), arg5.data()));
                break;
            }
        }

        return QtHelper::variantToValue(value);
    }

private:

    /**
     * \brief Extract the name of a meta method
     *
     * \param metaMethod Qt meta method
     *
     * \return Name of the meta method
     */
    static std::string functionName(const QMetaMethod& metaMethod)
    {
        QString signature = metaMethod.signature();
        return signature.left(signature.indexOf('(')).toStdString();
    }

    /**
     * \brief Extract the return type of a meta method
     *
     * \param metaMethod Qt meta method
     *
     * \return Return type of the meta method
     */
    static camp::Type returnType(const QMetaMethod& metaMethod)
    {
        return QtHelper::type(metaMethod.typeName());
    }

    /**
     * \brief Extract the arguments types of a meta method
     *
     * \param metaMethod Qt meta method
     *
     * \return Arguments types of the meta method
     */
    static std::vector<camp::Type> argTypes(const QMetaMethod& metaMethod)
    {
        std::vector<camp::Type> types;

        QList<QByteArray> args = metaMethod.parameterTypes();
        Q_FOREACH(QByteArray arg, args)
        {
            types.push_back(QtHelper::type(arg));
        }

        return types;
    }

private:

    QMetaMethod m_metaMethod; ///< Internal Qt method
};

} // namespace camp_ext


#endif // CAMP_QT_QTFUNCTION_HPP
