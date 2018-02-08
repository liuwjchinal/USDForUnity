using System;
using UnityEngine;

namespace UTJ.USD
{

    [Serializable]
    public class UsdXform : UsdSchema
    {
        #region fields
        [SerializeField] protected Transform m_trans;

        usdi.Xform m_xf;
        usdi.XformData m_xfData = usdi.XformData.default_value;
        protected usdi.UpdateFlags m_updateFlags;
        #endregion

        #region properties
        public usdi.Xform nativeXformPtr
        {
            get { return m_xf; }
        }
        #endregion

        #region impl
        protected override UsdIComponent usdiSetupSchemaComponent()
        {
            return GetOrAddComponent<UsdXformComponent>();
        }

        public override void UsdOnLoad()
        {
            base.UsdOnLoad();
            m_xf = usdi.usdiAsXform(m_schema);
            m_trans = GetComponent<Transform>();
        }

        public override void UsdOnUnload()
        {
            base.UsdOnUnload();
            m_xf = default(usdi.Xform);
        }

        public override void UsdAsyncUpdate(double time)
        {
            m_updateFlags = usdi.usdiPrimGetUpdateFlags(m_xf);
            usdi.usdiXformReadSample(m_xf, ref m_xfData, time);
        }

        public override void UsdUpdate(double time)
        {
            base.UsdUpdate(time);
            if(m_goAssigned)
            {
                usdi.TransformAssign(m_trans, ref m_xfData);
            }
        }
        #endregion
    }

}
